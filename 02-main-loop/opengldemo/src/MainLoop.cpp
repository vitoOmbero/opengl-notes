#include "MainLoop.h"
#include "utils/MacroAsText.inc"

#include <cstdlib>

GLFWwindow* MainLoop::win_ = nullptr;
MainLoop*   MainLoop::obj_ = nullptr;

void MainLoop::Start()
{
    // BEHOLD the MAIN LOOP
    while (!glfwWindowShouldClose(win_))
    {
        glfwPollEvents();

        glClearColor(.24, .12, .24, 1.);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(win_);
    }
}

MainLoop::~MainLoop()
{
    delete obj_;
    glfwTerminate();
}

MainLoop::MainLoop(const char* title)
{
    if (!glfwInit())
    {
        Terminal::Report("GLFW initialization failed");
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    win_ = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title, nullptr, nullptr);

    if (nullptr == win_)
    {
        Terminal::Report("Window initialization failed");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(win_);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        Terminal::Report("GLEW initialization failed");
        std::exit(EXIT_FAILURE);
    }
}
