#include "gl_rendering_target.h"

size_t SizeOfDataPack(const VertexAttributeData* vad, size_t index)
{

    auto n_attributes = vad->n_vertex_attribute_packs;

    auto n_datapacks = vad->n_attribute_data_packs;

    size_t result = 0;
    if (n_datapacks == 1)
    {
        // interleaved

        // TODO; move to VertexAttributeData?

        for (size_t i = 0; i < n_attributes; ++i)
        {
            result +=
                SizeOfGlTypeByGLenum(vad->vap[i].data_type) *
                attribute::RuntimeMap::Get(vad->vap_scheme[i])->attributes_n;
        }
    }
    else
    {
        // contiguous

        result +=
            SizeOfGlTypeByGLenum(vad->vap[index].data_type) *
            attribute::RuntimeMap::Get(vad->vap_scheme[index])->attributes_n;
        ;
    }

    return result * vad->adp->n_vertices;
}
