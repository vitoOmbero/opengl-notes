#include "mesh.h"

#include <algorithm>

#include "utils/gl_enum.h"

Mesh::Mesh(VertexAttributeData* vad, std::string vertex_shader_src,
           std::string fragment_shader_src, DrawingSpec descriptor)
    : vad_{ vad }
    , descriptor_{ descriptor }
{
    // TODO: auto detect if indices are needed

    v_shader_src_ = vertex_shader_src;
    f_shader_src_ = fragment_shader_src;
}

Mesh::Mesh(VertexAttributeData* vad, IndexDataPack* ind,
           std::string vertex_shader_src, std::string fragment_shader_src,
           DrawingSpec descriptor)
    : vad_{ vad }
    , ind_{ ind }
    , v_shader_src_{ vertex_shader_src }
    , f_shader_src_{ fragment_shader_src }
    , descriptor_{ descriptor }
{
}

Mesh::Mesh(const Mesh& another)
    : vad_{ another.vad_ }
    , ind_{ another.ind_ }
    , v_shader_src_{ another.v_shader_src_ }
    , f_shader_src_{ another.f_shader_src_ }
    , descriptor_{ another.descriptor_ }
{
}

Mesh& Mesh::operator=(const Mesh& another)
{
    // TODO: where is the resourse magament?

    this->vad_          = another.vad_;
    this->ind_          = another.ind_;
    this->v_shader_src_ = another.v_shader_src_;
    this->f_shader_src_ = another.f_shader_src_;
    this->descriptor_   = another.descriptor_;
    return *this;
}

// NOTE:     how about to really free resourses?
Mesh::Mesh(Mesh&& another)
    : vad_{ another.vad_ }
    , ind_{ another.ind_ }
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
        this->ind_          = another.ind_;
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

size_t Mesh::getSizeOfDataPack(size_t index) const
{

    auto n_attributes =
        this->getVertexAttributeData()->n_vertex_attribute_packs;

    auto n_datapacks = this->getVertexAttributeData()->n_attribute_data_packs;

    size_t result = 0;
    if (n_datapacks == 1)
    {
        // interleaved

        // TODO; move to VertexAttributeData?

        for (size_t i = 0; i < n_attributes; ++i)
        {
            result +=
                SizeOfGlTypeByGLenum(this->getVertexAttributeData()
                                         ->vap[i]
                                         .data_pack.data_type) *
                this->getVertexAttributeData()->vap[i].n_attributes_in_pack;
        }
    }
    else
    {
        // contiguous

        result +=
            SizeOfGlTypeByGLenum(this->getVertexAttributeData()
                                     ->vap[index]
                                     .data_pack.data_type) *
            this->getVertexAttributeData()->vap[index].n_attributes_in_pack;
    }

    return result * this->getVertexAttributeData()->adp->n_vertices;
}
