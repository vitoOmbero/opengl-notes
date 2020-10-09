#include "demo.h"
#include "utils/macro_as_text.inc"

#include <cstdlib>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl_renderer.h"
#include "input_controller.h"
#include "scene.h"
#include "window.h"

bool is_initialized = false;

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

Window *window;

Demo::~Demo() {}

void Demo::Init(const char *title, bool fullscreen)
{
    if (!is_initialized) {
        InputController::Init();
        window = new Window(WIN_WIDTH, WIN_HEIGHT, title, fullscreen);
        is_initialized = true;
    } else
        Terminal::ReportErr("Demo is already initialized.");
}

void Demo::Terminate()
{
    delete window;

    glfwTerminate();
}

Demo::Demo() {}

void Demo::Start()
{
    Scene simple_triangle;

    GlRenderer renderer(simple_triangle);

    // BEHOLD the MAIN LOOP
    while (!window->ShouldClose()) {
        window->ShowFpsInTitle();
        glfwPollEvents();
        renderer.Render();
        window->SwapBuffers();
    }
}
