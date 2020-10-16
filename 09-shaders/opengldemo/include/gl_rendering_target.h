#ifndef MESH_DESCRIPTORS_H
#define MESH_DESCRIPTORS_H

#include <cstddef>

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "attribute.h"
#include "utils/gl_enum.h"

enum class AttributeDataLayout : char
{
    kContiguous,
    kInterleaved // <-- recommended
};

struct AttributeDataPack
{
    const void*   data;
    const GLsizei n_vertices;
    const AttributeDataLayout layout;
    const unsigned char       associated_vap_n;
    ///> should be FALSE if data values ARE normilized
    const bool is_NOT_data_normalized_flag{ false };
    ///> in case if some VAP use the same ADP, but several (up to 8) do not use
    /// idices (no need to dublicte ADP data structures). First from associated
    /// VAPs flags is << 0, second: << 1, etc.
    const unsigned char shared_adp__indices_usage_flags{ 0 };
    const GLenum        indices_data_type{ 0 };
    const GLsizei       indices_n{ 0 };
    const char*         indices_data{ nullptr };
};

struct VertexAttributePack
{
    /// each attribute may have it's different type for data
    ///> data type of attribute value as gl symbolic type description
    GLenum data_type;
    ///> number of bytes between two instances of the attribute pack
    const GLsizei stride{ 0 };
    ///> number of bytes between start position of vertex data pack
    /// and start position of the attribute pack
    const GLvoid* offset = nullptr;
};

// NOTE: All OpenGL ES 2.0 implementations must support a minimum of eight
// vertex attributes.
inline static const unsigned char kAttributesMax{ 8 };
inline static const unsigned char kVapSchemeArrSize{ kAttributesMax + 5 };
/**
 * @brief The VertexAttributeData struct Implicitly supports 1:N pack mapping
 */
#pragma pack(push, 8)
struct VertexAttributeData
{
    /**
     * @brief The CompositionCase enum Explicit specification of data structure
     * usage
     */
    enum class CompositionCase : unsigned char
    {
        k11,
        k1N,
        kArr,
        kIllFormed
    };

    const AttributeDataPack*   adp;
    const VertexAttributePack* vap;
    ///> schemas for packs
    const attribute::Scheme vap_scheme[kVapSchemeArrSize];
    // NOTE: next fields can be removed into the VertexAttributeData creation
    // context, so vap_schemes can be [16]
    const unsigned char   n_attribute_data_packs;
    const unsigned char   n_vertex_attribute_packs;
    const CompositionCase composition;
};
#pragma pack(pop)

class GlRenderingTarget
{
public:
private:
};

size_t SizeOfDataPack(const VertexAttributeData* vad, size_t index);

#endif // MESH_DESCRIPTORS_H
