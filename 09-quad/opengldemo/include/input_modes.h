#ifndef INPUT_MODES_H
#define INPUT_MODES_H

#include <GLFW/glfw3.h>

#include <utils/terminal.h>

template<class T>
struct InputMode
{
    static void KeyCallback(GLFWwindow *win, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, GL_TRUE);
#ifdef KEY_INPUT_VERBOSE
            Terminal::ReportMsg("Pressed: Esc");
#endif
        }

        T::Callback(win, key, scancode, action, mods);
    };
};

struct DemoScene : public InputMode<DemoScene>
{
    inline static bool gWireframe = false;

    // TODO: who should serve input commands?

    static void Callback(GLFWwindow *win, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            if (key == GLFW_KEY_W && action == GLFW_PRESS) {
                gWireframe = !gWireframe;
                if (gWireframe)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
#ifdef KEY_INPUT_VERBOSE
            Terminal::ReportMsg("Pressed: W");
#endif
        }
    }
};

struct TriangleScene : public InputMode<TriangleScene>
{
    static void Callback(GLFWwindow *win, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            // ... no actions yet ...
#ifdef KEY_INPUT_VERBOSE
            Terminal::ReportMsg("Pressed: <-");
#endif
        }
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            // ... no actions yet ...
#ifdef KEY_INPUT_VERBOSE
            Terminal::ReportMsg("Pressed: <-");
#endif
        }
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            // ... no actions yet ...
#ifdef KEY_INPUT_VERBOSE
            Terminal::ReportMsg("Pressed: <-");
#endif
        }
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            // ... no actions yet ...
#ifdef KEY_INPUT_VERBOSE
            Terminal::ReportMsg("Pressed: <-");
#endif
        }
    }
};

#endif // INPUT_MODES_H
