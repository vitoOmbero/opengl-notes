#ifndef LOOP_H
#define LOOP_H

#include "vector"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "attribute.h"
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
    GLuint               vap_binding_counter_;

    /**
     * @brief CreateAndBindVertexBufferObjects Processes rendering targets
     * @param vao_name Vertex array object name for vertex buffer objects be
     * binded
     * @param vad pointer to Vertex Attribute Data object
     * @param ds Drawing specification for rendering targets
     */
    void CreateAndBindVertexBufferObjects(const GLuint&              vao_name,
                                          const VertexAttributeData* vad,
                                          DrawingSpec                ds);

    void CookShaderProgram(const Mesh& mesh);
};

#endif // LOOP_H
