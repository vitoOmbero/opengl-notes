#include "mesh.h"

#include <algorithm>

#include "attribute.h"

Mesh::Mesh(VertexAttributeData* vad, std::string vertex_shader_src,
           std::string fragment_shader_src, DrawingSpec descriptor)
    : vad_{ vad }
    , descriptor_{ descriptor }
{
    // TODO: auto detect if indices are needed

    v_shader_src_ = vertex_shader_src;
    f_shader_src_ = fragment_shader_src;
}

Mesh::Mesh(const Mesh& another)
    : vad_{ another.vad_ }
    , v_shader_src_{ another.v_shader_src_ }
    , f_shader_src_{ another.f_shader_src_ }
    , descriptor_{ another.descriptor_ }
{
}

Mesh& Mesh::operator=(const Mesh& another)
{
    // TODO: where is the resourse magament?

    this->vad_          = another.vad_;
    this->v_shader_src_ = another.v_shader_src_;
    this->f_shader_src_ = another.f_shader_src_;
    this->descriptor_   = another.descriptor_;
    return *this;
}

// NOTE:     how about to really free resourses?
Mesh::Mesh(Mesh&& another)
    : vad_{ another.vad_ }
    , v_shader_src_{ another.v_shader_src_ }
    , f_shader_src_{ another.f_shader_src_ }
    , descriptor_{ another.descriptor_ }
{
}

Mesh& Mesh::operator=(Mesh&& another)
{
    if (this != &another)
    {
        this->vad_          = another.vad_;
        this->v_shader_src_ = another.v_shader_src_;
        this->f_shader_src_ = another.f_shader_src_;
        this->descriptor_   = another.descriptor_;
    }
    return *this;
}

DrawingSpec Mesh::getDrawingSpec() const
{
    return descriptor_;
}

void Mesh::setDescriptor(const DrawingSpec& descriptor)
{
    descriptor_ = descriptor;
}

std::string Mesh::getFragmentShaderSrc() const
{
    return f_shader_src_;
}

std::string Mesh::getVertexShaderSrc() const
{
    return v_shader_src_;
}

VertexAttributeData const* Mesh::getVertexAttributeData() const
{
    return vad_;
}
