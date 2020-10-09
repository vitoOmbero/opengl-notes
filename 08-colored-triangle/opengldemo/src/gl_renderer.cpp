#include "gl_renderer.h"

GlRenderer::GlRenderer(Window *win, Scene &scene)
    : win_{win}, vbo_{scene.getVbo()}, vao_{scene.getVao()}, shader_program_{
                                                                 scene.getShaderProgram()}
{
    glClearColor(.24, .12, .24, 1.);
}

void GlRenderer::Render()
{
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    // Render the triangle
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    win_->SwapBuffers();
}

GlRenderer::~GlRenderer()
{
    glDeleteProgram(shader_program_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}
