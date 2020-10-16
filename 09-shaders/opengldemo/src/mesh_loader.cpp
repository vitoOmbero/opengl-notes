#include "mesh_loader.h"

#include "utils/gl_enum.h"

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

static AttributeDataPack adp{ /*data*/ reinterpret_cast<char*>(data),
                              /*n_vertices*/ 3,
                              /*layout*/ AttributeDataLayout::kContiguous,
                              /*associated_vap_n*/ 1 };

static VertexAttributePack vap{ GL_FLOAT };

static VertexAttributeData vad{
    &adp, &vap, { attribute::Scheme::kPosition3d },
    1,    1,    VertexAttributeData::CompositionCase::k11
};

const char* v_shader_src_path =
    "assets/shaders/demo/simple/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangle

namespace demo::assets::SimpleTriangleInterleaved
{
// former TriangleObjectPureLayout
static float data[] // clang-format off
        /*data self*/ { 0.f,  0.5f, 0.f,	/* Top   */   1.f, 0.f, 0.f, /*Red channel*/
                        0.5f, -0.5f, 0.f,	/* Right */   0.f, 1.f, 0.f, /*Green channel*/
                        -0.5f, -0.5f, 0.f,	/* Left  */   0.f, 0.f, 1.f, /*Blue channel*/
        };
// clang-format on

static AttributeDataPack adp{ reinterpret_cast<char*>(data), 3,
                              AttributeDataLayout::kInterleaved, 1 };

static VertexAttributePack vap[]{
    /*[0] - positions*/
    { GL_FLOAT,
      /*stride*/ 6 * (GLsizei)SizeOfGlTypeByGLenum(GL_FLOAT) },
    /*[1] - colors*/
    { GL_FLOAT,
      /*stride*/ 6 * (GLsizei)SizeOfGlTypeByGLenum(GL_FLOAT),
      /*offset*/ (GLfloat*)(sizeof(GLfloat) * 3) }
};

static VertexAttributeData vad{ &adp,
                                vap,
                                { attribute::Scheme::kPosition3d,
                                  attribute::Scheme::kColorRgb },
                                /*n_adp*/ 1,
                                /*n_vap*/ 2,
                                VertexAttributeData::CompositionCase::k1N };

const char* v_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangleInterleaved

namespace demo::assets::SimpleTriangleContiguous
{
// former TriangleObjectPureLayout - 2 vbo for one triangle (2 rendering
// targets)
static float vertex_position_data[] // clang-format off
        /*data self*/ { 0.f,  0.5f, 0.f,	/* Top   */
                        0.5f, -0.5f, 0.f,	/* Right */
                        -0.5f, -0.5f, 0.f,	/* Left  */
        };
// clang-format on

static float vertex_color_data[] // clang-format off
        /*data self*/ { 1.f, 0.f, 0.f, /*Red channel*/
                        0.f, 1.f, 0.f, /*Green channel*/
                        0.f, 0.f, 1.f, /*Blue channel*/
        };
// clang-format on

static AttributeDataPack adp[]{ { reinterpret_cast<char*>(vertex_position_data),
                                  3, AttributeDataLayout::kContiguous, 1 },
                                { reinterpret_cast<char*>(vertex_color_data), 3,
                                  AttributeDataLayout::kContiguous,
                                  /*assoc_vap_n*/ 1 } };

static VertexAttributePack vap[]{
    /*[0] - positions*/ { GL_FLOAT }, /*[1] - colors*/ { GL_FLOAT }
}; // namespace demo::assets::SimpleTriangleContiguous

static VertexAttributeData vad{ adp,
                                vap,
                                { attribute::Scheme::kPosition3d,
                                  attribute::Scheme::kColorRgb },
                                /*n_adp*/ 2,
                                /*n_vap*/ 2,
                                VertexAttributeData::CompositionCase::kArr };

// shaders are the same as for SimpleTriangleInterleaved
const char* v_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangleContiguous

namespace demo::assets::SimpleQuadUnindexed
{
const char* v_shader_src_path =
    "assets/shaders/demo/simple_indices/simple_tr_for_quad.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_indices/simple_tr_for_unind_quad.f.glsl";

static float data[] // clang-format off
        /*data self*/ {
		// First triangle
		-0.5f,  0.5f, 0.0f,	// Top left			<---- duplicate 1
		 0.5f,  0.5f, 0.0f,	// Top right 
	     0.5f, -0.5f, 0.0f,	// Bottom right		<---- duplicate 2

	   // Second triangle
	    -0.5f,  0.5f, 0.0f,	// Top left			<---- duplicate 1
	     0.5f, -0.5f, 0.0f,	// Bottom right		<---- duplicate 2
	    -0.5f, -0.5f, 0.0f	// Bottom Left	
        };
// clang-format on

static AttributeDataPack adp{ reinterpret_cast<char*>(data), 3 + 3,
                              AttributeDataLayout::kInterleaved, 1 };

VertexAttributePack vap{ GL_FLOAT };

static VertexAttributeData vad{
    &adp, &vap, { attribute::Scheme::kPosition3d },
    1,    1,    VertexAttributeData::CompositionCase::k11
};

} // namespace demo::assets::SimpleQuadUnindexed

namespace demo::assets::SimpleQuadIndexed
{
const char* v_shader_src_path =
    "assets/shaders/demo/simple_indices/simple_tr_for_quad.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_indices/simple_tr_for_ind_quad.f.glsl";

static float v_data[] // clang-format off
        /*data self*/ {
		-0.5f,  0.5f, 0.0f,		// Top left
		 0.5f,  0.5f, 0.0f,		// Top right
		 0.5f, -0.5f, 0.0f,		// Bottom right
		-0.5f, -0.5f, 0.0f		// Bottom left 
        };

static int i_data[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};
// clang-format on

static AttributeDataPack adp{ reinterpret_cast<char*>(v_data),
                              3 + 1,
                              AttributeDataLayout::kInterleaved,
                              1,
                              false,
                              0,
                              GL_UNSIGNED_INT,
                              6,
                              reinterpret_cast<char*>(i_data) };

VertexAttributePack vap{GL_FLOAT};

static VertexAttributeData vad{
    &adp, &vap, { attribute::Scheme::kPosition3d },
    1,    1,    VertexAttributeData::CompositionCase::k11
};

} // namespace demo::assets::SimpleQuadIndexed

MeshLoader::MeshLoader()
    : meshes()
{
}

void MeshLoader::Load(DemoAssets demo_asset)
{
    using namespace demo::assets;

    switch (demo_asset)
    {
        case DemoAssets::kSimpleTriangle_1vbo_1att:
        {
            using namespace SimpleTriangle;

            std::string vsrc = FileToString(v_shader_src_path);
            std::string fsrc = FileToString(f_shader_src_path);

            Mesh simple_triangle(&vad, vsrc, fsrc);
            meshes.push_back(simple_triangle);
            break;
        }

        case DemoAssets::kSimpleTriangle_1vbo_2att_Interleaved:
        {
            using namespace SimpleTriangleInterleaved;

            std::string vsrc = FileToString(v_shader_src_path);
            std::string fsrc = FileToString(f_shader_src_path);

            Mesh simple_triangle(&vad, vsrc, fsrc);
            meshes.push_back(simple_triangle);
            break;
        }
        case DemoAssets::kSimpleTriangle_2vbo_2att_Contiguous:
        {
            using namespace SimpleTriangleContiguous;

            std::string vsrc = FileToString(v_shader_src_path);
            std::string fsrc = FileToString(f_shader_src_path);

            Mesh simple_triangle(&vad, vsrc, fsrc);
            meshes.push_back(simple_triangle);
            break;
        }

        case DemoAssets::kSimpleQuad_Unindexed:
        {
            using namespace SimpleQuadUnindexed;

            std::string vsrc = FileToString(v_shader_src_path);
            std::string fsrc = FileToString(f_shader_src_path);

            Mesh simple_triangle(&vad, vsrc, fsrc);
            meshes.push_back(simple_triangle);
            break;
        }

        case DemoAssets::kSimpleQuad_Indexed:
        {
            using namespace SimpleQuadIndexed;

            std::string vsrc = FileToString(v_shader_src_path);
            std::string fsrc = FileToString(f_shader_src_path);

            Mesh simple_triangle(&vad, vsrc, fsrc);
            meshes.push_back(simple_triangle);
            break;
        }

        default:
        {
            Terminal::ReportErr("Attempt to use an unknown demo asset");
        }
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
