#ifndef WINDOW_H
#define WINDOW_H
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, std::string title, bool fullscreen);

    void SwapBuffers();
    bool ShouldClose();
    void ShowFpsInTitle();

    ~Window();

private:
    int width_;
    int height_;
    bool is_fullscreen_ = false;
    GLFWwindow *win_ = nullptr;
    GLFWmonitor *mon_ = nullptr;
    std::string title_ = std::string();

    void CreateWindow();
};

#endif // WINDOW_H
