#ifndef ASSETS_H
#define ASSETS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using GlSymbolicConstant = int;

namespace attributes {

enum Type { kPosition, kColor };

enum Values {
    kPosNormalizedXCoord,
    kPosNormalizedYCoord,
    kPosNormalizedZCoord,

    kColRedChannel,
    kColGreenChannel,
    kColBlueChannel,
    kColAlphaChannel
};

/* actually no need in runtime map
#include <map>
const std::multimap<Type, Values> dictionary{
    {kPosition, kPosNormalizedXCoord},
    {kPosition, kPosNormalizedYCoord},
    {kPosition, kPosNormalizedZCoord},

    {kColor, kColRedChannel},
    {kColor, kColGreenChannel},
    {kColor, kColBlueChannel},
    {kColor, kColAlphaChannel},
};
*/

namespace schemas {
const Values position_2d[]{kPosNormalizedXCoord, kPosNormalizedYCoord};
const Values position_3d[]{kPosNormalizedXCoord, kPosNormalizedYCoord, kPosNormalizedZCoord};
const Values color_rgb[]{kColRedChannel, kColGreenChannel, kColBlueChannel};
const Values color_rgba[]{kColRedChannel, kColGreenChannel, kColBlueChannel, kColAlphaChannel};
} // namespace schemas

} // namespace attributes

/**
 * @brief SizeOfGlTypeBySymbolicConstant
 * @param gl_symb_constant GL_FLOAT, GL_FIXED, etc 
 * @return sizeof(gl_associated_type) or 0
 */
inline size_t SizeOfGlTypeBySymbolicConstant(int gl_symb_constant)
{
    switch (gl_symb_constant) {
    case GL_FLOAT:
        return sizeof(GLfloat);
    case GL_BYTE:
        return sizeof(GLbyte);
    case GL_UNSIGNED_BYTE:
        return sizeof(GLubyte);
    case GL_SHORT:
        return sizeof(GLshort);
    case GL_UNSIGNED_SHORT:
        return sizeof(GLushort);
    case GL_HALF_FLOAT:
        return sizeof(GLhalf);
    case GL_INT_2_10_10_10_REV:
    case GL_INT:
        return sizeof(GLint);
    case GL_FIXED:
        return sizeof(GLfixed);
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        return sizeof(GLuint);
    default:
        return 0;
    }
};

inline int GlBooleanSymbolicConstant(bool arg)
{
    return arg ? GL_TRUE : GL_FALSE;
}

struct VertexAttributePack
{
#pragma pack(push, 1)
    struct DataPack
    {
        GlSymbolicConstant data_type; ///> data type of attribute value as gl symbolic type description
        bool normalized_data_flag;    ///> should be true if data values are normilized
    };
#pragma pack(pop)

    const attributes::Type type;              ///> type of pack
    const GLint n_attributes_in_pack;         ///> number of attributes in pack
    const attributes::Values *attributes_arr; ///> attributes in pack
    const size_t stride; ///> number of bytes between two instances of the attribute pack
    const GLvoid *offset
        = nullptr; ///> number of bytes between start position of vertex data pack and start position of the attribute pack
    const DataPack data_pack; ///> data description for attribute pack
};

struct VertexAttributeData
{
    void *data; ///> pointer to data
    size_t n_attribute_types;       ///> == number of packs for one vertex
    VertexAttributePack *packs_arr; ///> data description
};

/**
 * @brief The DemoTriangleObject struct One buffer object with 1 attribute to draw one 
 */
struct DemoTriangleObject
{
    const char *v_shader_src = R"GLSL(
#version 330

layout (location = 0) in vec3 pos;

void main(){
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);
}
)GLSL";
    const char *f_shader_src = R"GLSL(
#version 330

out vec4 color;

void main(){
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
)GLSL";

    GLfloat vertex_data[9] // clang-format off
        /*data self*/ { 0.0f,  0.5f, 0.0f,	// Top
                        0.5f, -0.5f, 0.0f,	// Right 
                        -0.5f, -0.5f, 0.0f,	// Left
        };
    // clang-format on

    GLsizeiptr n_vertices = 3; // number of vertices described in vertex_data

    VertexAttributePack vertex_attr_pack{
        /*type*/ attributes::Type::kPosition,
        /*n_attributes_in_pack*/ sizeof(attributes::schemas::position_3d)
            / sizeof(attributes::Values),
        /*attributes_arr*/ attributes::schemas::position_3d,
        /*stride*/ 0,
        /*offset*/ nullptr,
        /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}};

    VertexAttributeData vertex_attribute_data{/*data*/ vertex_data,
                                              /*attribute types*/ 1,
                                              /*packs: just 1*/
                                              &vertex_attr_pack};
};

/**
 * @brief The TriangleObjectInterleavedLayout struct One buffer object with 2 attributes to draw one
 */
struct TriangleObjectInterleavedLayout
{
    const char *v_shader_src = R"GLSL(
#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 vertex_color;

void main(){
    vertex_color = col;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.f);
}
)GLSL";
    const char *f_shader_src = R"GLSL(
#version 330

in vec3 vertex_color;
out vec4 color;

void main(){
    color = vec4(vertex_color, 1.f);
}
)GLSL";

    GLfloat vertex_data[9 + 9] // clang-format off
        /*data self*/ { 0.f,  0.5f, 0.f,	/* Top   */   1.f, 0.f, 0.f, /*Red channel*/
                        0.5f, -0.5f, 0.f,	/* Right */   0.f, 1.f, 0.f, /*Green channel*/
                        -0.5f, -0.5f, 0.f,	/* Left  */   0.f, 0.f, 1.f, /*Blue channel*/
        };
    // clang-format on

    VertexAttributePack vertex_attr_pack[2]{
        // clang-format off

            // position attribute pack description

            {/*type*/ attributes::Type::kPosition,
             /*n_attributes_in_pack*/ sizeof(attributes::schemas::position_3d) / sizeof(attributes::Values),
             /*attributes_arr*/ attributes::schemas::position_3d,        
             /*stride*/ 6 * SizeOfGlTypeBySymbolicConstant(GL_FLOAT),
             /*offset*/ nullptr,
             /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}
            },

            // color attribute pack description

            { /*type*/ attributes::Type::kColor,
              /*n_attributes_in_pack*/ sizeof(attributes::schemas::color_rgb) / sizeof(attributes::Values),
              /*attributes_arr*/ attributes::schemas::color_rgb,
              /*stride*/ 6 * SizeOfGlTypeBySymbolicConstant(GL_FLOAT),
              /*offset*/ (GLfloat *)(sizeof(GLfloat)* 3),
              /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}      
            } // clang-format on

    };

    VertexAttributeData vertex_attribute_data{/*data*/ vertex_data,
                                              /*attribute types*/ 2,
                                              /*packs: now 2*/
                                              vertex_attr_pack};
};

/**
 * @brief The TriangleObjectSerializedLayout struct Two buffer objects with 1 attribute to draw one
 */
struct TriangleObjectSerializedLayout
{
    const char *v_shader_src = R"GLSL(
#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 vertex_color;

void main(){
    vertex_color = col;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.f);
}
)GLSL";
    const char *f_shader_src = R"GLSL(
#version 330

in vec3 vertex_color;
out vec4 color;

void main(){
    color = vec4(vertex_color, 1.f);
}
)GLSL";

    GLfloat vertex_position_data[9] // clang-format off
        /*data self*/ { 0.f,  0.5f, 0.f,	/* Top   */
                        0.5f, -0.5f, 0.f,	/* Right */
                        -0.5f, -0.5f, 0.f,	/* Left  */
        };
    // clang-format on

    VertexAttributePack vertex_position_attr_pack{
        /*type*/ attributes::Type::kPosition,
        /*n_attributes_in_pack*/ sizeof(attributes::schemas::position_3d)
            / sizeof(attributes::Values),
        /*attributes_arr*/ attributes::schemas::position_3d,
        /*stride*/ 0,
        /*offset*/ nullptr,
        /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}};

    VertexAttributeData vertex_position_attribute_data{/*data*/ vertex_position_data,
                                                       /*attribute types*/ 1,
                                                       /*packs: just 1*/
                                                       &vertex_position_attr_pack};

    GLfloat vertex_color_data[9] // clang-format off
        /*data self*/ { 1.f, 0.f, 0.f, /*Red channel*/
                        0.f, 1.f, 0.f, /*Green channel*/
                        0.f, 0.f, 1.f, /*Blue channel*/
        };
    // clang-format on

    VertexAttributePack vertex_color_attr_pack{
        /*type*/ attributes::Type::kColor,
        /*n_attributes_in_pack*/ sizeof(attributes::schemas::color_rgb) / sizeof(attributes::Values),
        /*attributes_arr*/ attributes::schemas::color_rgb,
        /*stride*/ 0,
        /*offset*/ nullptr,
        /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}};

    VertexAttributeData vertex_color_attribute_data{/*data*/ vertex_color_data,
                                                    /*attribute types*/ 1,
                                                    /*packs: now 1*/
                                                    &vertex_color_attr_pack};
};

#endif // ASSETS_H
