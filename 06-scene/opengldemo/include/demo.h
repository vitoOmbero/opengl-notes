#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "utils/terminal.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Demo
{
public:
    static void Init(const char* title, bool fullscreen = false);

    static void Start();

    ~Demo();

private:
    Demo();
};

#endif // MAINLOOP_H
