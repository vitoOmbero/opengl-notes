#include "gl_rendering_target.h"

#include <algorithm>
#include <numeric>

#include "utils/terminal.h"

size_t gl_rendering_target::SizeOfVertexData(
        const gl_rendering_target::RenderingTargetPackPointer rtpp, size_t index)
{

    auto vdp = rtpp.vdp[index];

    std::vector<size_t> sz;
    sz.reserve(rtpp.n_aps);

    for (size_t i = 0; i < rtpp.n_aps; ++i)
    {

        AttributePackSpecification* r{nullptr};

        for (size_t j=i; j < rtpp.n_aps; j++){
            if (rtpp.aps[j].rendering_target_id == vdp.rendering_target_id){
                r = &rtpp.aps[j];
            }
        }

        if (nullptr == r){
            Terminal::ReportErr("Bad AttributePackSpecification!");
        }

        sz.emplace_back(SizeOfGlTypeByGLenum(r->type_code) *
                        attribute::RuntimeMap::Get(r->scheme)->attributes_n);
    }

    auto result = std::accumulate(sz.begin(), sz.end(), 0ul) * vdp.n_vertices;

    return result;
}

size_t gl_rendering_target::SizeOfIndexData(
        const VertexDataPointer *const vdp)
{
    return SizeOfGlTypeByGLenum(vdp->i_typecode) * vdp->n_indices;
}
