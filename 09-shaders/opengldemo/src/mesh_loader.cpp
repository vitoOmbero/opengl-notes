#include "mesh_loader.h"

#include "utils/gl_enum.h"

#include "demo_assets.h"
#include "utils/terminal.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "utils/sequence_generator.h"

namespace demo::assets::SimpleTriangle
{
static uint32_t id = gRenderingTargetIdGen.getNext();
using namespace gl_rendering_target;
// former TriangleObjectPureLayout

static float data[] // clang-format off
        /*data self*/ { 0.0f,  0.5f, 0.0f,	// Top
                        0.5f, -0.5f, 0.0f,	// Right 
                        -0.5f, -0.5f, 0.0f,	// Left
        };
// clang-format on

static VertexDataPointer vdp = VertexDataPointer(id, 3, data);

static AttributePackSpecification aps =
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kContiguous,
                               attribute::Scheme::kPosition3d);

static RenderingTargetPackPointer rtpp{ &vdp, &aps, 1, 1 };

const char* v_shader_src_path =
    "assets/shaders/demo/simple/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangle

namespace demo::assets::SimpleTriangleInterleaved
{
static uint32_t id = gRenderingTargetIdGen.getNext();
using namespace gl_rendering_target;

// former TriangleObjectPureLayout
static float data[] // clang-format off
        /*data self*/ { 0.f,  0.5f, 0.f,	/* Top   */   1.f, 0.f, 0.f, /*Red channel*/
                        0.5f, -0.5f, 0.f,	/* Right */   0.f, 1.f, 0.f, /*Green channel*/
                        -0.5f, -0.5f, 0.f,	/* Left  */   0.f, 0.f, 1.f, /*Blue channel*/
        };
// clang-format on

static VertexDataPointer vdp = VertexDataPointer(id, 3, data);

static AttributePackSpecification aps[] = {
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kInterleaved,
                               attribute::Scheme::kPosition3d, nullptr,
                               6 * (GLsizei)SizeOfGlTypeByGLenum(GL_FLOAT)),
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kInterleaved,
                               attribute::Scheme::kColorRgb,
                               (GLfloat*)(sizeof(GLfloat) * 3),
                               6 * (GLsizei)SizeOfGlTypeByGLenum(GL_FLOAT))
};

static RenderingTargetPackPointer rtpp{ &vdp, aps, 1, 2 };

const char* v_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangleInterleaved

namespace demo::assets::SimpleTriangleContiguous
{
static uint32_t id = gRenderingTargetIdGen.getNext();
using namespace gl_rendering_target;
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

static VertexDataPointer vdp[]{ VertexDataPointer(id, 3, vertex_position_data),
                                VertexDataPointer(id, 3, vertex_color_data) };

static AttributePackSpecification aps[] = {
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kContiguous,
                               attribute::Scheme::kPosition3d),
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kContiguous,
                               attribute::Scheme::kColorRgb)
};

static RenderingTargetPackPointer rtpp{ vdp, aps, 2, 2 };

// shaders are the same as for SimpleTriangleInterleaved
const char* v_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_interleaved/simple_triangle.f.glsl";
} // namespace demo::assets::SimpleTriangleContiguous

namespace demo::assets::SimpleQuadUnindexed
{
static uint32_t id = gRenderingTargetIdGen.getNext();
using namespace gl_rendering_target;

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

static VertexDataPointer vdp = VertexDataPointer(id, 3 + 3, data);

static AttributePackSpecification aps =
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kInterleaved,
                               attribute::Scheme::kPosition3d);

static RenderingTargetPackPointer rtpp{ &vdp, &aps, 1, 1 };

} // namespace demo::assets::SimpleQuadUnindexed

namespace demo::assets::SimpleQuadIndexed
{
static uint32_t id = gRenderingTargetIdGen.getNext();
using namespace gl_rendering_target;

const char* v_shader_src_path =
    "assets/shaders/demo/simple_indices/simple_tr_for_quad.v.glsl";
const char* f_shader_src_path =
    "assets/shaders/demo/simple_indices/simple_tr_for_ind_quad.f.glsl";

static GLfloat v_data[] // clang-format off
        /*data self*/ {
		-0.5f,  0.5f, 0.0f,		// Top left
		 0.5f,  0.5f, 0.0f,		// Top right
		 0.5f, -0.5f, 0.0f,		// Bottom right
		-0.5f, -0.5f, 0.0f		// Bottom left 
        };

static GLuint i_data[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};
// clang-format on

static VertexDataPointer vdp =
    VertexDataPointer(id, 4, v_data, 6, GL_UNSIGNED_INT, i_data);

static AttributePackSpecification aps =
    AttributePackSpecification(id, GL_FLOAT, AttributeDataLayout::kContiguous,
                               attribute::Scheme::kPosition3d);

static RenderingTargetPackPointer rtpp{ &vdp, &aps, 1, 1 };

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

            meshes.emplace_back(&rtpp, FileToString(v_shader_src_path),
                                FileToString(f_shader_src_path));
            break;
        }

        case DemoAssets::kSimpleTriangle_1vbo_2att_Interleaved:
        {
            using namespace SimpleTriangleInterleaved;

            meshes.emplace_back(&rtpp, FileToString(v_shader_src_path),
                                FileToString(f_shader_src_path));
            break;
        }
        case DemoAssets::kSimpleTriangle_2vbo_2att_Contiguous:
        {
            using namespace SimpleTriangleContiguous;

            meshes.emplace_back(&rtpp, FileToString(v_shader_src_path),
                                FileToString(f_shader_src_path));
            break;
        }

        case DemoAssets::kSimpleQuad_Unindexed:
        {
            using namespace SimpleQuadUnindexed;

            meshes.emplace_back(&rtpp, FileToString(v_shader_src_path),
                                FileToString(f_shader_src_path));
            break;
        }

        case DemoAssets::kSimpleQuad_Indexed:
        {
            using namespace SimpleQuadIndexed;

            meshes.emplace_back(&rtpp, FileToString(v_shader_src_path),
                                FileToString(f_shader_src_path));
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
