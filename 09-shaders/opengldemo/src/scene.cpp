#include "scene.h"

Scene::Scene()
    : ml_()
{

    // ml_.Load(DemoAssets::kSimpleTriangle_1vbo_1att);
    // ml_.Load(DemoAssets::kSimpleTriangle_1vbo_2att_Interleaved);
    // ml_.Load(DemoAssets::kSimpleTriangle_2vbo_2att_Contiguous);
    // ml_.Load(DemoAssets::kSimpleQuad_Unindexed);
    ml_.Load(DemoAssets::kSimpleQuad_Indexed);
}

MeshLoader& Scene::getMeshLoader()
{
    return ml_;
}
