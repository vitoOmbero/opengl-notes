#include "main_loop.h"
#include "utils/macro_as_text.inc"

#include <cstdlib>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "input_controller.h"

// file - forward declarations - refactoring mark

void ShowFpsInWinTitle(GLFWwindow* win, const char* title);
// refactoring mark

GLFWwindow*  win_           = nullptr;
MainLoop *loop_single_ = nullptr;
GLFWmonitor* mon_           = nullptr;
std::string  title_         = std::string();
bool         is_fullscreen_ = false;

void MainLoop::Init(const char* title, bool fullscreen)
{
    is_fullscreen_ = fullscreen;
    if (nullptr == loop_single_) {
        loop_single_ = new MainLoop();
        title_ = std::string(title);
    } else
        Terminal::ReportErr("MainLoop is already initialized.");
}

void MainLoop::Start()
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

    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(
        0, // Attribute index, "0".  The vertex shader should have layout "0" for the position of the vertex
        3,        // Number of components of attribute "0". In this case 3 floats for x,y,z
        GL_FLOAT, // The data type of each component
        GL_FALSE, // Normalize component values to [-1, 1]? No, not for floating point component types
        0, // Stride, number of bytes between two instances of the attribute in the buffer. This buffer is "Tightly packed"
        NULL); // Offset inside the structure to find the attribute

    glEnableVertexAttribArray(0); // Enable the first attribute or attribute "0"

    //  Create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &v_shader, NULL);
    glCompileShader(vs);

    // Check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // 4. Create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &f_shader, NULL);
    glCompileShader(fs);

    // Check for compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // 5. Create shader program and link shaders to program
    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    // Check for linker errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // Clean up shaders, do not need them anymore since they are linked to a shader program
    glDeleteShader(vs);
    glDeleteShader(fs);

    glClearColor(.24, .12, .24, 1.);

    // BEHOLD the MAIN LOOP
    while (!glfwWindowShouldClose(win_))
    {
        ShowFpsInWinTitle(win_, title_.data());
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // Render the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(win_);
    }
    // Clean up
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

MainLoop::~MainLoop()
{
    delete loop_single_;
    glfwTerminate();
}

MainLoop::MainLoop()
{
    if (!glfwInit())
    {
        Terminal::ReportErr("GLFW initialization failed");
        std::exit(EXIT_FAILURE);
    }

    InputController::Init();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mon_ = glfwGetPrimaryMonitor();
    CreateWindow();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        Terminal::ReportErr("GLEW initialization failed");
        std::exit(EXIT_FAILURE);
    }
}

void FrameBuferrSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MainLoop::CreateWindow()
{
    if (!is_fullscreen_)
    {
        if (nullptr != win_)
        {
            glfwDestroyWindow(win_);
        }
        win_ = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title_.data(), nullptr,
                                nullptr);
    }
    else
    {
        const GLFWvidmode* mode = glfwGetVideoMode(mon_);
        if (nullptr != mode)
        {
            if (nullptr != win_)
            {
                glfwDestroyWindow(win_);
            }
            win_ = glfwCreateWindow(mode->width, mode->height, title_.data(),
                                    mon_, nullptr);
        }
    }

    if (nullptr == win_)
    {
        Terminal::ReportErr("Window initialization failed");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(win_);

    glfwSetKeyCallback(win_, InputController::KeyCallback);
    glfwSetFramebufferSizeCallback(win_, FrameBuferrSizeCallback);

    FrameBuferrSizeCallback(win_, WIN_WIDTH, WIN_HEIGHT);
}

void ShowFpsInWinTitle(GLFWwindow* win, const char* title)
{
    static double prev_secs   = 0.;
    static int    frame_count = 0;
    double        elapsed_secs;
    double        current_secs = glfwGetTime();

    elapsed_secs = current_secs - prev_secs;
    // update 4 times per sec
    if (elapsed_secs > .25)
    {
        prev_secs           = current_secs;
        double fps = (double)frame_count / elapsed_secs;
        double ms_per_frame = 1000. / fps;

        std::ostringstream oss;
        oss.precision(4);

        oss << (title == nullptr ? " " : title) << std::fixed
            << " [[FPS: " << fps << "\t"
            << "ms/frame:" << ms_per_frame << "]]";
        glfwSetWindowTitle(win, oss.str().data());

        frame_count = 0;
    }

    ++frame_count;
}
