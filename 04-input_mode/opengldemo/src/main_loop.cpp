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
    // BEHOLD the MAIN LOOP
    while (!glfwWindowShouldClose(win_))
    {
        ShowFpsInWinTitle(win_, title_.data());
        glfwPollEvents();

        glClearColor(.24, .12, .24, 1.);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(win_);
    }
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
