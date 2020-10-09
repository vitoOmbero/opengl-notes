#ifndef LOOP_H
#define LOOP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "window.h"

class GlRenderer
{
public:
    /**
     * @brief Loop Main demo loop
     * @param window Loop breaks when window is going to close
     * @param pre_render_actions Callbacks are executed before rendering
     */
    GlRenderer(Window *win_, Scene &scene);

    /**
     * @brief Start Loop starts and checks the statement at each iteration
     */
    void Render();

    ~GlRenderer();

private:
    Window *win_;
    GLuint vbo_;
    GLuint vao_;
    GLint shader_program_;
};

#endif // LOOP_H
