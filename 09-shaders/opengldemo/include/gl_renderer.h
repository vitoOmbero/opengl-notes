#ifndef LOOP_H
#define LOOP_H

#include "vector"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh_loader.h"
#include "window.h"

class GlRenderer
{
public:
    GlRenderer();

    // TODO:  iterators are needed only
    void Init(MeshLoader& ml);

    /**
     * @brief Start Loop starts and checks the statement at each iteration
     */
    void RenderFrame();

    ~GlRenderer();

private:
    std::vector<GLuint> vbo_;
    std::vector<GLuint> ebo_;
    std::vector<GLuint> shaders_;
    ///> vertices or indices number
    std::vector<GLsizei> target_sizes;
    GLuint               vao_;
    GLuint               vao_binding_counter_;

    /**
     * @brief CreateVbo
     * @param mesh
     * @return number of created vbo
     */
    size_t CreateVbo(const Mesh& mesh);
    void   BindVbo(const Mesh& mesh, size_t vbo_count,
                   GLuint vao_counter_before_vbo_creation);

    void CookShaderProgram(const Mesh& mesh);
};

#endif // LOOP_H
