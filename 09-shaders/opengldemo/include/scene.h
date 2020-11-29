#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "demo_assets.h"
#include "mesh_loader.h"
#include "utils/gl_check.h"
#include "utils/terminal.h"

class Scene
{
public:
    Scene();

    MeshLoader& getMeshLoader();

private:
    MeshLoader ml_;
};

#endif // SCENE_H
