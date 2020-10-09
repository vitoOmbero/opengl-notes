#include "input_controller.h"
#include "input_modes.h"

InputController *impl_obj_;

GLFWkeyfun InputController::KeyCallback;

void InputController::Init()
{
    impl_obj_ = new InputController();
}

InputController::~InputController()
{
    delete impl_obj_;
}

InputController::InputController()
{
    SetMode<DemoScene>();
}
