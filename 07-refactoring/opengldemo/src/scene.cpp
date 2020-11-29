#include "scene.h"

#include "utils/terminal.h"

Scene::Scene()
{
    // draw one object preparation (temporary, code is here as assets are statically determinated)

    glGenBuffers(1, &vbo_); // returned value can be random and wil be uniqur for each object
    glBindBuffer(
        GL_ARRAY_BUFFER,
        vbo_); // the vertex array pointer parameter is interpreted as an offset within the buffer object measured in basic machine units

    glBufferData(GL_ARRAY_BUFFER, sizeof(object.vertex_data), object.vertex_data, GL_STATIC_DRAW); /*
    GL_ARRAY_BUFFER - target    
    STATIC - The data store contents will be modified once and used many times.
    DRAW - The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.    
*/
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // processing attribute packs - just 1 in this case

    glVertexAttribPointer(
        0, // Attribute index, "0". Each vertex is described with only one attribute in this case (with 1 attribute pack, if we talk in the local domain problem terms)
        object.vertex_attribute_data.packs_arr[0]
            .n_attributes_in_pack, // Number of components of attribute "0". In this case 3 floats for x,y,z
        object.vertex_attribute_data.packs_arr[0]
            .data_pack.data_type, // The data type of each component
        GlBooleanSymbolicConstant(
            !object.vertex_attribute_data.packs_arr[0]
                 .data_pack
                 .normalized_data_flag), // there is no need to normilize if data is already normilized
        object.vertex_attribute_data.packs_arr[0].stride, // 0 if "Tightly packed"
        NULL); // Offset inside the structure to find the attribute

    glEnableVertexAttribArray(0); // Enable the first attribute or attribute "0"

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &object.v_shader_src, NULL); // NULL tells that strings are null-terminated
    glCompileShader(vs);

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
    glShaderSource(fs, 1, &object.f_shader_src, NULL);
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

    // draw one object preparation FINISHED

#ifdef SCENE_VERBOSE
    Terminal::ReportMsg("Object preparation finished.");
#endif
}

GLuint Scene::getVbo() const
{
    return vbo_;
}

GLuint Scene::getVao() const
{
    return vao_;
}

GLint Scene::getShaderProgram() const
{
    return shader_program_;
}
