#ifndef MESH_DESCRIPTORS_H
#define MESH_DESCRIPTORS_H

#include <cstddef>
#include <string>

#include <algorithm>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "attribute.h"
#include "utils/gl_enum.h"


namespace gl_rendering_target
{
    enum class AttributeDataLayout : char
    {
        kContiguous,
        kInterleaved // <-- recommended
    };

    inline static const size_t kVertexAttributesGlMax = 16;
    inline static const size_t kVertexAttributesEsMax = 8;

    struct VertexDataPointer
    {
        uint32_t rendering_target_id;
        GLsizei  n_vertices;
        void*    v_data;
        GLsizei  n_indices;
        GLenum   i_typecode;
        void*    i_data;

        VertexDataPointer() = default;
        VertexDataPointer(uint32_t rendering_target_id, GLsizei n_vertices,
                          void* v_data, GLsizei n_indices = 0, GLenum i_typecode = 0,
                          void* i_data = nullptr)
                : rendering_target_id{ rendering_target_id }
                , n_vertices{ n_vertices }
                , v_data{ v_data }
                , n_indices{n_indices }
                , i_typecode{ i_typecode }
                , i_data{ i_data } {};
    };

    struct AttributePackSpecification
    {
        uint32_t            rendering_target_id;
        GLenum              type_code;
        GLvoid*             offset;
        GLsizei             stride;
        AttributeDataLayout layout;
        attribute::Scheme   scheme;
        bool                is_data_NOT_normalized;

        AttributePackSpecification() = default;
        AttributePackSpecification(uint32_t rendering_target_id, GLenum type_code,
                                   AttributeDataLayout layout,
                                   attribute::Scheme   scheme,
                                   GLvoid* offset = nullptr, GLsizei stride = 0,
                                   bool is_data_NOT_normalized = false)
                : rendering_target_id{ rendering_target_id }
                , type_code{type_code }
                , offset{ offset }
                , stride{ stride }
                , layout{ layout }
                , scheme{ scheme }
                , is_data_NOT_normalized{ is_data_NOT_normalized } {};
    };

    struct RenderingTargetPackPointer
    {
        VertexDataPointer*          vdp;
        AttributePackSpecification* aps;
        size_t                      n_vdp;
        size_t                      n_aps;
    };

    struct RenderingTargetPackFixed
    {
        VertexDataPointer          vdp[kVertexAttributesEsMax];
        AttributePackSpecification aps[kVertexAttributesEsMax];
    };

    enum class CompositionCase : unsigned char
    {
        ///> rendering target is described with 1 adp and 1 vap
        k11,
        ///> rendering target is described with 1 adp and N vap
        k1N,
        ///> rendering target is described with M adp and N vap
        kMn,
        ///> several rendering targets
        kArr,
        kIllFormed
    };

    size_t SizeOfVertexData(const RenderingTargetPackPointer rtpp, size_t index);
    size_t SizeOfIndexData(const VertexDataPointer *const vdp);

    struct ShaderPack
    {
        std::string vertex_src;
        std::string fragment_src;
    };
} // namespace gl_rendering_target

#endif // MESH_DESCRIPTORS_H
