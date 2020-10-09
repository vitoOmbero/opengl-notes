#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "utils/Terminal.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MainLoop
{
public:
    static void Init(const char* title)
    {
        if (nullptr == obj_)
            obj_ = new MainLoop(title);
        else
            Terminal::Report("MainLoop is already initialized.");
    };

    static void Start();

    ~MainLoop();

private:
    MainLoop(const char* title);

    static const int WIN_WIDTH  = 800;
    static const int WIN_HEIGHT = 600;

    static GLFWwindow* win_;
    static MainLoop*   obj_;
};

#endif // MAINLOOP_H
