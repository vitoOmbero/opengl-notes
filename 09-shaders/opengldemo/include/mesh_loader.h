#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "vector"

#include "demo_assets.h"
#include "mesh.h"

class MeshLoader
{
public:
    MeshLoader();

    void Load(DemoAssets demo_asset);

    using MeshIterator = std::vector<Mesh>::iterator;

    MeshIterator begin();
    MeshIterator end();

private:
    std::vector<Mesh> meshes;

    std::string FileToString(const std::string& filename);
};

#endif // MESHLOADER_H
