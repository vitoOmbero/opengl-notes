#include "loop.h"

Loop::Loop(GLFWwindow *window, LoopAction pre_render_actions[]) : win_{window}
{
    pre_render_actions_[0] = pre_render_actions[0];
    pre_render_actions_[1] = pre_render_actions[1];
}

void Loop::Start(GLuint vbo, GLuint vao, GLint shader_program)
{
    // BEHOLD the MAIN LOOP
    while (!glfwWindowShouldClose(win_)) {
        pre_render_actions_[0]();
        pre_render_actions_[1]();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // Render the triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(win_);
    }
    // Clean up
    glDeleteProgram(shader_program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
