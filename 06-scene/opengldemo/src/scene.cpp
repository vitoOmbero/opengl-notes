#include "scene.h"

#include "utils/terminal.h"

Scene::Scene()
{
    static const char *v_shader = R"GLSL(
#version 330

layout (location = 0) in vec3 pos;

void main(){
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);
}
)GLSL";

    static const char *f_shader = R"GLSL(
#version 330

out vec4 color;

void main(){
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
)GLSL";

    // clang-format off
    GLfloat vertices[] = {
        0.0f,  0.5f, 0.0f,	// Top
        0.5f, -0.5f, 0.0f,	// Right 
	    -0.5f, -0.5f, 0.0f,	// Left
    };
    // clang-format on

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glVertexAttribPointer(
        0,        // Attribute index, "0".  The vertex shader should have layout "0" for the position of the vertex
        3,        // Number of components of attribute "0". In this case 3 floats for x,y,z
        GL_FLOAT, // The data type of each component
        GL_FALSE, // Normalize component values to [-1, 1]? No, not for floating point component types
        0,        // Stride, number of bytes between two instances of the attribute in the buffer. This buffer is "Tightly packed"
        NULL);    // Offset inside the structure to find the attribute

    glEnableVertexAttribArray(0); // Enable the first attribute or attribute "0"

    //  Create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &v_shader, NULL);
    glCompileShader(vs);

    // Check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // Create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &f_shader, NULL);
    glCompileShader(fs);

    // Check for compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // Create shader program and link shaders to program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // Check for linker errors
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader_program_, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // Clean up shaders, do not need them anymore since they are linked to a shader program
    glDeleteShader(vs);
    glDeleteShader(fs);

    glClearColor(.24, .12, .24, 1.);
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
