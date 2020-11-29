#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cassert>

#include "assets.h"
#include "utils/terminal.h"

#define GL_CHECK() \
    { \
        const unsigned int err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            switch (err) { \
            case GL_INVALID_ENUM: \
                Terminal::ReportErr("GL_INVALID_ENUM"); \
                break; \
            case GL_INVALID_VALUE: \
                Terminal::ReportErr("GL_INVALID_VALUE"); \
                break; \
            case GL_INVALID_OPERATION: \
                Terminal::ReportErr("GL_INVALID_OPERATION"); \
                break; \
            case GL_INVALID_FRAMEBUFFER_OPERATION: \
                Terminal::ReportErr("GL_INVALID_FRAMEBUFFER_OPERATION"); \
                break; \
            case GL_OUT_OF_MEMORY: \
                Terminal::ReportErr("GL_OUT_OF_MEMORY"); \
                break; \
            } \
            assert(false); \
        } \
    }

class Scene
{
public:
    Scene();

    GLuint getVbo() const;

    GLuint getVao() const;

    GLint getShaderProgram() const;

    GLsizeiptr getNumIndices() const;

    bool getIndex_draw() const;

private:
    GLuint vbo_;
    GLuint vao_;
    GLint shader_program_;
    GLsizeiptr n_indices_;

    TriangleObjectPureLayout tr_object_pl;
    void cook_one_object_one_vbo_one_attribute();

    TriangleObjectInterleavedLayout object_il;
    void cook_one_object_one_vbo_two_attributes();

    TriangleObjectSerializedLayout object_sl;
    void cook_one_object_two_vbo_one_attribute_in_each();

    ObjectPureLayout1a<9 + 9> quad_pure;
    ObjectIndexedLayout1a<3 * 4, 3 * 2> quad_ind;

    template<size_t n>
    void cook(ObjectPureLayout1a<n> &obj);

    template<size_t n, GLintptr in>
    void cook(ObjectIndexedLayout1a<n, in> &obj);

    bool index_draw = false;
};

template<size_t n, GLintptr in>
void Scene::cook(ObjectIndexedLayout1a<n, in> &obj)
{
    index_draw = true; // whae draw with indices Renderer uses another Draw gl function

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obj.vertex_data), obj.vertex_data, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // processing attribute packs - just 1 in this case

    glVertexAttribPointer(0,
                          obj.vertex_attribute_data.packs_arr[0].n_attributes_in_pack,
                          obj.vertex_attribute_data.packs_arr[0].data_pack.data_type,
                          GlBooleanSymbolicConstant(!obj.vertex_attribute_data.packs_arr[0]
                                                         .data_pack.normalized_data_flag),
                          obj.vertex_attribute_data.packs_arr[0].stride,
                          NULL);
    GL_CHECK()
    glEnableVertexAttribArray(0);
    GL_CHECK()

    GLuint ibo;

    // Set up index buffer
    glGenBuffers(1, &ibo); // Create buffer space on the GPU for the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(obj.index_data), obj.index_data, GL_STATIC_DRAW);

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &obj.v_shader_src, NULL);
    GL_CHECK()
    glCompileShader(vs);
    GL_CHECK()

    // check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &obj.f_shader_src, NULL);
    glCompileShader(fs);

    // check
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create shader program and link shaders to program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // check
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader_program_, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // clean up shaders, they are linked already
    glDeleteShader(vs);
    glDeleteShader(fs);

    n_indices_ = obj.n_indices;
    // draw one object preparation FINISHED
}

template<size_t n>
void Scene::cook(ObjectPureLayout1a<n> &obj)
{
    glGenBuffers(1, &vbo_);
    GL_CHECK()
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    GL_CHECK()

    glBufferData(GL_ARRAY_BUFFER, sizeof(obj.vertex_data), obj.vertex_data, GL_STATIC_DRAW);
    GL_CHECK()
    glGenVertexArrays(1, &vao_);
    GL_CHECK()
    glBindVertexArray(vao_);
    GL_CHECK()

    // processing attribute packs - just 1 in this case

    glVertexAttribPointer(0,
                          obj.vertex_attribute_data.packs_arr[0].n_attributes_in_pack,
                          obj.vertex_attribute_data.packs_arr[0].data_pack.data_type,
                          GlBooleanSymbolicConstant(!obj.vertex_attribute_data.packs_arr[0]
                                                         .data_pack.normalized_data_flag),
                          obj.vertex_attribute_data.packs_arr[0].stride,
                          NULL);
    GL_CHECK()
    glEnableVertexAttribArray(0);
    GL_CHECK()

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &obj.v_shader_src, NULL);
    GL_CHECK()
    glCompileShader(vs);
    GL_CHECK()

    // check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &obj.f_shader_src, NULL);
    glCompileShader(fs);

    // check
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create shader program and link shaders to program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // check
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader_program_, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // clean up shaders, they are linked already
    glDeleteShader(vs);
    glDeleteShader(fs);

    n_indices_ = obj.n_indices;
    // draw one object preparation FINISHED
}

#endif // SCENE_H
