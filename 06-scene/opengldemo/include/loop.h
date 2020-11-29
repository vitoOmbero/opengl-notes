#ifndef LOOP_H
#define LOOP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using LoopAction = void (*)();
static const size_t PRE_RENDER_ACTIONS_MAX = 2;

class Loop
{
public:
    /**
     * @brief Loop Main demo loop
     * @param window Loop breaks when window is going to close
     * @param pre_render_actions Callbacks are executed before rendering
     */
    Loop(GLFWwindow *window, LoopAction pre_render_actions[]);

    /**
     * @brief Start Loop starts and checks the statement at each iteration
     */
    void Start(GLuint vbo, GLuint vao, GLint shader_program);

private:
    GLFWwindow *win_;
    LoopAction pre_render_actions_[PRE_RENDER_ACTIONS_MAX];
};

#endif // LOOP_H
