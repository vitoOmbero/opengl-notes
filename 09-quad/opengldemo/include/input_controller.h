#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <string_view>

#include <GLFW/glfw3.h>

#include "input_modes.h"

class InputController
{
public:
    static void Init();

    template<class T>
    static void SetMode();

    static GLFWkeyfun KeyCallback;

    ~InputController();

private:
    InputController();
};

template<class T>
void InputController::SetMode()
{
    KeyCallback = InputMode<T>::KeyCallback;
}

#endif // INPUTCONTROLLER_H
