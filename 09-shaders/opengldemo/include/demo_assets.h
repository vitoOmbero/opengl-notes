#ifndef ASSETS_H
#define ASSETS_H

#include "mesh_descriptors.h"
#include "utils/gl_enum.h"

/**
 * @brief The TriangleObjectInterleavedLayout struct One buffer object with 2
 * attributes to draw one
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

            {/*type*/ attribute::Type::kPosition,
             /*n_attributes_in_pack*/ sizeof(attribute::scheme::position_3d) / sizeof(attribute::Value),
             /*attributes_arr*/ attribute::scheme::position_3d,
             /*stride*/ 6 * (GLint)SizeOfGlTypeByGLenum(GL_FLOAT),
             /*offset*/ nullptr,
             /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}
            },

            // color attribute pack description

            { /*type*/ attribute::Type::kColor,
              /*n_attributes_in_pack*/ sizeof(attribute::scheme::color_rgb) / sizeof(attribute::Value),
              /*attributes_arr*/ attribute::scheme::color_rgb,
              /*stride*/ 6 * (GLint)SizeOfGlTypeByGLenum(GL_FLOAT),
              /*offset*/ (GLfloat *)(sizeof(GLfloat)* 3),
              /*data_pack*/ {/*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true}      
            } // clang-format on

    };
    /*
        VertexAttributeData vertex_attribute_data{ 1, 2, &vertex_data,
                                                   vertex_attr_pack };*/
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
        /*type*/ attribute::Type::kPosition,
        /*n_attributes_in_pack*/ sizeof(attribute::scheme::position_3d) /
            sizeof(attribute::Value),
        /*attributes_arr*/ attribute::scheme::position_3d,
        /*stride*/ 0,
        /*offset*/ nullptr,
        /*data_pack*/
        { /*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true }
    };

    VertexAttributeData vertex_position_attribute_data{
        //      1, 1, vertex_position_data, &vertex_position_attr_pack
    };

    GLfloat vertex_color_data[9] // clang-format off
        /*data self*/ { 1.f, 0.f, 0.f, /*Red channel*/
                        0.f, 1.f, 0.f, /*Green channel*/
                        0.f, 0.f, 1.f, /*Blue channel*/
        };
    // clang-format on

    VertexAttributePack vertex_color_attr_pack{
        /*type*/ attribute::Type::kColor,
        /*n_attributes_in_pack*/ sizeof(attribute::scheme::color_rgb) /
            sizeof(attribute::Value),
        /*attributes_arr*/ attribute::scheme::color_rgb,
        /*stride*/ 0,
        /*offset*/ nullptr,
        /*data_pack*/
        { /*GlSymbolicConstant*/ GL_FLOAT, /*normalized_data_flag*/ true }
    };

    /* VertexAttributeData vertex_color_attribute_data{ 1, 1, vertex_color_data,
                                                      &vertex_color_attr_pack
       };*/
};

template<size_t v_data_size>
struct ObjectPureLayout1a
{
    ObjectPureLayout1a(const char *v_shader_src,
                       const char *f_shader_src,
                       GLfloat v_data[],
                       GLsizeiptr n_indices,
                       VertexAttributeData vertex_attribute_data)
        : v_shader_src{v_shader_src}, f_shader_src{f_shader_src}, n_indices{n_indices},
          vertex_attribute_data{vertex_attribute_data}
    {
        for (size_t i = 0; i < v_data_size; ++i) {
            vertex_data[i] = v_data[i];
        }
    };

    const char *v_shader_src;
    const char *f_shader_src;
    const GLsizeiptr n_indices;
    const VertexAttributeData vertex_attribute_data;
    /*const*/ GLfloat vertex_data[v_data_size]; // note: this is problem - no more C style
};

template<size_t v_data_size, GLsizeiptr num_indices>
struct ObjectIndexedLayout1a
{
    ObjectIndexedLayout1a(const char *v_shader_src,
                          const char *f_shader_src,
                          GLfloat v_data[],
                          VertexAttributeData vertex_attribute_data,
                          GLuint i_data[])
        : v_shader_src{v_shader_src}, f_shader_src{f_shader_src}, n_indices{num_indices},
          vertex_attribute_data{vertex_attribute_data}
    {
        for (size_t i = 0; i < v_data_size; ++i) {
            vertex_data[i] = v_data[i];
        }
        for (size_t i = 0; i < num_indices; ++i) {
            index_data[i] = i_data[i];
        }
    };

    const char *v_shader_src;
    const char *f_shader_src;
    const GLsizeiptr n_indices;
    const VertexAttributeData vertex_attribute_data;
    /*const*/ GLfloat vertex_data[v_data_size]; // note: this is problem - no more C style
    GLuint index_data[num_indices];
};

#endif // ASSETS_H
