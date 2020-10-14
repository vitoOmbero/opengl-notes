#include "scene.h"

Scene::Scene()
    : ml_()
{

    ml_.Load(DemoAssets::kSimpleTriangle);
}

MeshLoader& Scene::getMeshLoader()
{
    return ml_;
}
