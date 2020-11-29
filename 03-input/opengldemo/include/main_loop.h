#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "utils/terminal.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MainLoop
{
public:
    static void Init(const char* title, bool fullscreen = false);

    static void Start();

    ~MainLoop();

private:
    MainLoop();
    static void CreateWindow();
    static void KeyCallback(GLFWwindow* win, int key, int scancode, int action,
                            int mods);

    static const int WIN_WIDTH  = 800;
    static const int WIN_HEIGHT = 600;
};

#endif // MAINLOOP_H
