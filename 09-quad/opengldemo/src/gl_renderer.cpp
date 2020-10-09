#include "gl_renderer.h"

static bool indices_draw = false;

GlRenderer::GlRenderer(Scene &scene)
    : vbo_{scene.getVbo()}, vao_{scene.getVao()}, shader_program_{scene.getShaderProgram()},
      n_indices_{scene.getNumIndices()}
{
    glClearColor(.24, .12, .24, 1.);

    indices_draw = scene.getIndex_draw();
}

void GlRenderer::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the triangle
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    if (!indices_draw)
        glDrawArrays(GL_TRIANGLES, 0, n_indices_);
    else
        glDrawElements(GL_TRIANGLES, n_indices_, GL_UNSIGNED_INT, 0); // 0 for offset
    glBindVertexArray(0); // TODO: change to glDisableArr
}

GlRenderer::~GlRenderer()
{
    // TODO: implement in Scene container and clean ALL
    glDeleteProgram(shader_program_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}
