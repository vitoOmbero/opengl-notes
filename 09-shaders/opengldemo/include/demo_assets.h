#ifndef ASSETS_H
#define ASSETS_H

#include "gl_rendering_target.h"
#include "utils/gl_enum.h"

enum class DemoAssets
{
    kSimpleTriangle_1vbo_1att,
    kSimpleTriangle_1vbo_2att_Interleaved,
    kSimpleTriangle_2vbo_2att_Contiguous,
    kSimpleQuad_Unindexed,
    kSimpleQuad_Indexed
};

#endif // ASSETS_H
