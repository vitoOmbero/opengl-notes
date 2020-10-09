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
    const DataPack data_pack;                 ///> data description for attribute pack
    const short stride; ///> number of bytes between two instances of the attribute
};

struct VertexAttributeData
{
    void *data; ///> pointer to data
    size_t n_attribute_types;
    VertexAttributePack *packs_arr; ///> data description
};

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
        /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true},
        /*stride*/ 0};

    VertexAttributeData vertex_attribute_data{/*data*/ vertex_data,
                                              /*attribute types*/ 1,
                                              /*packs: just 1*/
                                              &vertex_attr_pack};
};

#endif // ASSETS_H
