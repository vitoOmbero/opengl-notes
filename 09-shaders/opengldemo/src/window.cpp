#include "window.h"

#include <cstdlib>
#include <sstream>

#include "input_controller.h"
#include "utils/terminal.h"

void FrameBuferrSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ShowFpsInWinTitle(GLFWwindow *win, const char *title)
{
    static double prev_secs = 0.;
    static int frame_count = 0;
    double elapsed_secs;
    double current_secs = glfwGetTime();

    elapsed_secs = current_secs - prev_secs;
    // update 4 times per sec
    if (elapsed_secs > .25) {
        prev_secs = current_secs;
        double fps = (double) frame_count / elapsed_secs;
        double ms_per_frame = 1000. / fps;

        std::ostringstream oss;
        oss.precision(4);

        oss << (title == nullptr ? " " : title) << std::fixed << " [[FPS: " << fps << "\t"
            << "ms/frame:" << ms_per_frame << "]]";
        glfwSetWindowTitle(win, oss.str().data());

        frame_count = 0;
    }

    ++frame_count;
}

Window::Window(int width, int height, std::string title, bool fullscreen)
    : width_{width}, height_{height}, is_fullscreen_{fullscreen}, title_{title}
{
    if (!glfwInit()) {
        Terminal::ReportErr("GLFW initialization failed");
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    CreateWindow();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        Terminal::ReportErr("GLEW initialization failed");
        std::exit(EXIT_FAILURE);
    }
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(win_);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(win_);
}

void Window::ShowFpsInTitle()
{
    ShowFpsInWinTitle(win_, title_.data());
}

Window::~Window()
{
    glfwDestroyWindow(win_);
}

void Window::CreateWindow()
{
    if (!is_fullscreen_) {
        if (nullptr != win_) {
            glfwDestroyWindow(win_);
        }
        win_ = glfwCreateWindow(width_, height_, title_.data(), nullptr, nullptr);
    } else {
        const GLFWvidmode *mode = glfwGetVideoMode(mon_);
        if (nullptr != mode) {
            if (nullptr != win_) {
                glfwDestroyWindow(win_);
            }
            win_ = glfwCreateWindow(mode->width, mode->height, title_.data(), mon_, nullptr);
        }
    }

    if (nullptr == win_) {
        Terminal::ReportErr("Window initialization failed");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(win_);

    glfwSetKeyCallback(win_, InputController::KeyCallback);
    glfwSetFramebufferSizeCallback(win_, FrameBuferrSizeCallback);

    FrameBuferrSizeCallback(win_, width_, height_);

    mon_ = glfwGetPrimaryMonitor();
}
