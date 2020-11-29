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

    std::vector<uint32_t> v_target_id_;
    std::vector<GLuint>   v_target_shaders_;
    std::vector<GLsizei>  v_target_sizes;
    std::vector<gl_rendering_target::AttributePackSpecification> v_target_aps_;

    std::vector<uint32_t> i_target_id_;
    std::vector<GLuint>   i_target_shaders_;
    std::vector<GLsizei>  i_target_sizes_;
    std::vector<GLvoid*>  i_target_offsets_;
    size_t                accumulative_ebo_offset_{ 0 };

    GLuint               vao_;
    GLuint               aps_binding_counter_{1};

    /**
     * @brief ProcessMeshBoundedRenderingTargets Create vbo, ebo and register
     * all required rendering targets (implicitly)
     * @param vao_name Vertex array object name for vertex buffer objects be
     * binded
     * @param vad pointer to Vertex Attribute Data object
     * @param ds Drawing specification for rendering targets
     * @param sp Shaders will be registered for indexed target only
     */
    void ProcessMeshBoundedRenderingTargets(
        gl_rendering_target::RenderingTargetPackPointer rtpp, DrawingSpec ds,
        const gl_rendering_target::ShaderPack& sp);

    void CreateIndexedTarget(const gl_rendering_target::VertexDataPointer* vdp, DrawingSpec ds,
                             const gl_rendering_target::ShaderPack& sp);
    void CreateUnIndexedTarget(const gl_rendering_target::VertexDataPointer* vdp, DrawingSpec ds,
                             const gl_rendering_target::ShaderPack& sp);

    void CreateVbo();

    void CookShaderProgram(const gl_rendering_target::ShaderPack&    sp,
                           std::vector<GLuint>& shaders_container);

    void ProcessRenderingTargetK11(const gl_rendering_target::RenderingTargetPackPointer* rtpp,
                                   DrawingSpec                ds,
                                   unsigned char              target_index,
                                   const gl_rendering_target::ShaderPack&          sp);
    void ProcessRenderingTargetK1N(
            const gl_rendering_target::RenderingTargetPackPointer* rtpp, DrawingSpec ds,
            const gl_rendering_target::ShaderPack& sp);
};
#endif // LOOP_H
