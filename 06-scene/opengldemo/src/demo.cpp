#include "demo.h"
#include "utils/macro_as_text.inc"

#include <cstdlib>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "input_controller.h"
#include "loop.h"
#include "scene.h"

// file - forward declarations - refactoring mark

void ShowFpsInWinTitle(GLFWwindow* win, const char* title);
// refactoring mark

char demo[sizeof(Demo)];

GLFWwindow*  win_           = nullptr;
Demo *demo_ = nullptr;
GLFWmonitor* mon_           = nullptr;
std::string  title_         = std::string();
bool         is_fullscreen_ = false;

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

Demo::~Demo()
{
    delete demo_;
    glfwTerminate();
}

void FrameBuferrSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void CreateWindow()
{
    if (!is_fullscreen_) {
        if (nullptr != win_) {
            glfwDestroyWindow(win_);
        }
        win_ = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title_.data(), nullptr, nullptr);
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

    FrameBuferrSizeCallback(win_, WIN_WIDTH, WIN_HEIGHT);

    mon_ = glfwGetPrimaryMonitor();
}

void InitGl()
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

void Demo::Init(const char *title, bool fullscreen)
{
    is_fullscreen_ = fullscreen;
    if (nullptr == demo_) {
        demo_ = new (demo) Demo();
        title_ = std::string(title);
    } else
        Terminal::ReportErr("MainLoop is already initialized.");
}

Demo::Demo()
{
    InputController::Init();
    InitGl();
}

void ShowFpsLoopAction()
{
    ShowFpsInWinTitle(win_, title_.data());
}

void DoNothing(){
    // temporary solution
};

void Demo::Start()
{
    Scene simple_triangle;

    LoopAction arr[PRE_RENDER_ACTIONS_MAX]{ShowFpsLoopAction, DoNothing};

    Loop loop(win_, arr);

    loop.Start(simple_triangle.getVao(),
               simple_triangle.getVao(),
               simple_triangle.getShaderProgram());
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
