#ifndef MESH_DESCRIPTORS_H
#define MESH_DESCRIPTORS_H

#include <cstddef>

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "attribute.h"

enum class Layout
{
    kContiguous,
    kInterleaved // <-- recommended
};

struct AttributeDataPack
{
    const void*   data;
    const GLsizei n_vertices;
    const Layout  layout;
};

struct IndexDataPack
{
    GLenum        data_type;
    const GLsizei n_indices;
    const char*   data;
};

struct VertexAttributePack
{
/// each attribute may have it's different type for data
#pragma pack(push, 1)
    struct DataPack
    {
        ///> data type of attribute value as gl symbolic type description
        GLenum data_type;
        ///> should be true if data values are normilized
        bool normalized_data_flag;
    };
#pragma pack(pop)
    ///> type of pack
    const attribute::Type type;
    ///> number of attributes in pack
    const GLint n_attributes_in_pack;
    ///> specification of attributes in pack
    const attribute::Value* attributes_arr;
    ///> number of bytes between two instances of the attribute pack
    const GLsizei stride;
    ///> number of bytes between start position of vertex data pack
    /// and start position of the attribute pack
    const GLvoid* offset = nullptr;
    ///> data description for attribute pack
    const DataPack data_pack;
};

// NOTE: All OpenGL ES 2.0 implementations must support a minimum of eight
// vertex attributes.
struct VertexAttributeData
{
    const size_t               n_attribute_data_packs;
    const size_t               n_vertex_attribute_packs;
    const AttributeDataPack*   adp;
    const VertexAttributePack* vap;
};

inline static VertexAttributeData CreateInterleavedVertexAttributeData(
    size_t n_vertex_attribute_packs, AttributeDataPack* adp,
    VertexAttributePack* vap)
{
    return VertexAttributeData{ 1, n_vertex_attribute_packs, adp, vap };
};

inline static VertexAttributeData CreateContiguousVertexAttributeData(
    size_t n_vertex_attribute_packs, AttributeDataPack* adp,
    VertexAttributePack* vap)
{
    return VertexAttributeData{ n_vertex_attribute_packs,
                                n_vertex_attribute_packs, adp, vap };
};

struct BatchId
{
    unsigned int value;
};

#endif // MESH_DESCRIPTORS_H
