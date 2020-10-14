#include "mesh_loader.h"

#include "demo_assets.h"
#include "utils/terminal.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace demo::assets::SimpleTriangle
{

// former TriangleObjectPureLayout

static float data[] // clang-format off
        /*data self*/ { 0.0f,  0.5f, 0.0f,	// Top
                        0.5f, -0.5f, 0.0f,	// Right 
                        -0.5f, -0.5f, 0.0f,	// Left
        };
// clang-format on

static AttributeDataPack adp{ reinterpret_cast<char*>(data), 3,
                              Layout::kContiguous };

static VertexAttributePack vap{ attribute::Type::kPosition,
                                attribute::scheme::position_3d_attr_n,
                                attribute::scheme::position_3d,
                                /*stride*/ 0,
                                /*offset*/ nullptr,
                                /*data_pack*/
                                { /*GlSymbolicConstant*/
                                  GL_FLOAT,
                                  /*normalized_data_flag*/ true } };

static VertexAttributeData vad{ 1, 1, &adp, &vap };

const char* v_shader_src_path =
    "assets/shaders/demo/simple/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangle

MeshLoader::MeshLoader()
    : meshes()
{
}

void MeshLoader::Load(DemoAssets demo_asset)
{
    using namespace demo::assets;

    switch (demo_asset)
    {
        case DemoAssets::kSimpleTriangle:
        {

            std::string vsrc = FileToString(SimpleTriangle::v_shader_src_path);
            std::string fsrc = FileToString(SimpleTriangle::f_shader_src_path);

            Mesh simple_triangle(&SimpleTriangle::vad, vsrc, fsrc);
            meshes.push_back(simple_triangle);
            break;
        }

        default:
            break;
    }
}

MeshLoader::MeshIterator MeshLoader::begin()
{
    return meshes.begin();
}

MeshLoader::MeshIterator MeshLoader::end()
{
    return meshes.end();
}

std::string MeshLoader::FileToString(const std::string& filename)
{
    std::stringstream ss;
    std::ifstream     file;

    try
    {
        file.open(filename, std::ios::in);

        if (!file.fail())
        {
            ss << file.rdbuf();
        }

        file.close();
    }
    catch (std::exception ex)
    {
        Terminal::ReportErr("Error reading shader");
        Terminal::ReportErr(ex.what());
    }

    return ss.str();
}
