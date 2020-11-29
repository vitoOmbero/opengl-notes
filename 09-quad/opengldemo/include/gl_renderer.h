#ifndef LOOP_H
#define LOOP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "window.h"

class GlRenderer
{
public:
    GlRenderer(Scene &scene);

    /**
     * @brief Start Loop starts and checks the statement at each iteration
     */
    void Render();

    ~GlRenderer();

private:
    GLuint vbo_;
    GLuint vao_;
    GLint shader_program_;
    GLsizeiptr n_indices_;
};

#endif // LOOP_H
