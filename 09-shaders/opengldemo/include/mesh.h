#ifndef MESH_H
#define MESH_H

#include <string>

#include "mesh_descriptors.h"
#include "utils/gl_enum.h"

class Mesh
{
public:
    Mesh(VertexAttributeData* vad, std::string vertex_shader_src,
         std::string fragment_shader_src,
         DrawingSpec descriptor = DrawingSpec::kStatic);

    Mesh(VertexAttributeData* vad, IndexDataPack* ind,
         std::string vertex_shader_src, std::string fragment_shader_src,
         DrawingSpec descriptor);

    // NOTE: no, they won't be default all the time
    Mesh(const Mesh& another);
    Mesh& operator=(const Mesh& another);
    Mesh(Mesh&& another);
    Mesh& operator=(Mesh&& another);

    DrawingSpec getDrawingSpec() const;
    void        setDescriptor(const DrawingSpec& descriptor);

    std::string getFragmentShaderSrc() const;

    std::string getVertexShaderSrc() const;

    VertexAttributeData const* getVertexAttributeData() const;

    size_t getSizeOfDataPack(size_t index = 0) const;

private:
#pragma pack(push, 4)
    VertexAttributeData* vad_ = nullptr;
    IndexDataPack*       ind_ = nullptr;
    std::string          v_shader_src_;
    std::string          f_shader_src_;
    DrawingSpec          descriptor_;
#pragma pack(pop)
};

template <class T>
struct is_mesh
{
    static const bool value = false;
};

template <>
struct is_mesh<Mesh>
{
    static const bool value = true;
};
#endif // MESH_H
