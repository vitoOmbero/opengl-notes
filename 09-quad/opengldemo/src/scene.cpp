#include "scene.h"

namespace QuadPure {
const char *v_shader_src = R"GLSL(
#version 330

layout (location = 0) in vec3 pos;

void main(){
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)GLSL";
const char *f_shader_src = R"GLSL(
#version 330

out vec4 color;

void main(){
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
)GLSL";

const size_t vertex_data_size = 9 + 9;

GLfloat vertex_data[vertex_data_size] // clang-format off
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

GLsizeiptr n_indices = 3 + 3; // number of vertices described in vertex_data

VertexAttributePack
    vertex_attr_pack{/*type*/ attributes::Type::kPosition,
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
}; // namespace QuadPure

namespace QuadInd {
const char *v_shader_src = R"GLSL(
#version 330

layout (location = 0) in vec3 pos;

void main(){
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)GLSL";
const char *f_shader_src = R"GLSL(
#version 330

out vec4 color;

void main(){
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
)GLSL";

const size_t vertex_data_size = 3 * 4; // 3 attr, 4 vertices

const GLsizeiptr n_indices = 3 * 2; // 3 vertices, 2 triangles

GLfloat vertex_data[vertex_data_size] // clang-format off
        /*data self*/ {
		-0.5f,  0.5f, 0.0f,		// Top left
		 0.5f,  0.5f, 0.0f,		// Top right
		 0.5f, -0.5f, 0.0f,		// Bottom right
		-0.5f, -0.5f, 0.0f		// Bottom left 
        };

GLuint index_data[n_indices] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};
// clang-format on

VertexAttributePack
    vertex_attr_pack{/*type*/ attributes::Type::kPosition,
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
}; // namespace QuadInd

Scene::Scene()
    : quad_pure(QuadPure::v_shader_src,
                QuadPure::f_shader_src,
                QuadPure::vertex_data,
                QuadPure::n_indices,
                QuadPure::vertex_attribute_data),
      quad_ind(QuadInd::v_shader_src,
               QuadInd::f_shader_src,
               QuadInd::vertex_data,
               QuadInd::vertex_attribute_data,
               QuadInd::index_data)
{
    // draw first tringle with constant color in shader
    //cook_one_object_one_vbo_one_attribute();

    // draw colored triangle algorithm
    //cook_one_object_one_vbo_two_attributes();
    //cook_one_object_two_vbo_one_attribute_in_each();

    // draw quad
    //cook(quad_pure);
    cook(quad_ind);

#ifdef SCENE_VERBOSE
    Terminal::ReportMsg("Object preparation finished.");
#endif
}

GLuint Scene::getVbo() const
{
    return vbo_;
}

GLuint Scene::getVao() const
{
    return vao_;
}

GLint Scene::getShaderProgram() const
{
    return shader_program_;
}

GLsizeiptr Scene::getNumIndices() const
{
    return n_indices_;
}

void Scene::cook_one_object_one_vbo_one_attribute()
{
    glGenBuffers(1, &vbo_); // returned value can be random and wil be uniqur for each object
    glBindBuffer(
        GL_ARRAY_BUFFER,
        vbo_); // the vertex array pointer parameter is interpreted as an offset within the buffer object measured in basic machine units

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(tr_object_pl.vertex_data),
                 tr_object_pl.vertex_data,
                 GL_STATIC_DRAW); /*
    GL_ARRAY_BUFFER - target    
    STATIC - The data store contents will be modified once and used many times.
    DRAW - The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.    
*/
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // processing attribute packs - just 1 in this case

    glVertexAttribPointer(
        0, // Attribute index, "0". Each vertex is described with only one attribute in this case (with 1 attribute pack, if we talk in the local domain problem terms)
        tr_object_pl.vertex_attribute_data.packs_arr[0]
            .n_attributes_in_pack, // Number of components of attribute "0". In this case 3 floats for x,y,z
        tr_object_pl.vertex_attribute_data.packs_arr[0]
            .data_pack.data_type, // The data type of each component
        GlBooleanSymbolicConstant(
            !tr_object_pl.vertex_attribute_data.packs_arr[0]
                 .data_pack
                 .normalized_data_flag), // there is no need to normilize if data is already normilized
        tr_object_pl.vertex_attribute_data.packs_arr[0].stride, // 0 if "Tightly packed"
        NULL); // Offset inside the structure to find the attribute

    glEnableVertexAttribArray(0); // Enable the first attribute or attribute "0"

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,
                   1,
                   &tr_object_pl.v_shader_src,
                   NULL); // NULL tells that strings are null-terminated
    glCompileShader(vs);

    // check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &tr_object_pl.f_shader_src, NULL);
    glCompileShader(fs);

    // check
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create shader program and link shaders to program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // check
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader_program_, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // clean up shaders, they are linked already
    glDeleteShader(vs);
    glDeleteShader(fs);

    n_indices_ = tr_object_pl.n_indices;
    // draw one object preparation FINISHED
}

void Scene::cook_one_object_one_vbo_two_attributes()
{
    // one vbo, vao - the same code
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(object_il.vertex_data),
                 object_il.vertex_data,
                 GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // new shaders for colored triangle
    // now they contain info about location

    // position attribute
    glVertexAttribPointer(0,
                          object_il.vertex_attribute_data.packs_arr[0].n_attributes_in_pack,
                          object_il.vertex_attribute_data.packs_arr[0].data_pack.data_type,
                          GlBooleanSymbolicConstant(!object_il.vertex_attribute_data.packs_arr[0]
                                                         .data_pack.normalized_data_flag),
                          object_il.vertex_attribute_data.packs_arr[0].stride,
                          object_il.vertex_attribute_data.packs_arr[0].offset);
    glEnableVertexAttribArray(0);

    // color attribute

    glVertexAttribPointer(1,
                          object_il.vertex_attribute_data.packs_arr[1].n_attributes_in_pack,
                          object_il.vertex_attribute_data.packs_arr[1].data_pack.data_type,
                          GlBooleanSymbolicConstant(!object_il.vertex_attribute_data.packs_arr[1]
                                                         .data_pack.normalized_data_flag),
                          object_il.vertex_attribute_data.packs_arr[1].stride,
                          object_il.vertex_attribute_data.packs_arr[1].offset);
    glEnableVertexAttribArray(1);

    // they same code after

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,
                   1,
                   &object_il.v_shader_src,
                   NULL); // NULL tells that strings are null-terminated
    glCompileShader(vs);

    // check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &object_il.f_shader_src, NULL);
    glCompileShader(fs);

    // check
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create shader program and link shaders to program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // check
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader_program_, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // clean up shaders, they are linked already
    glDeleteShader(vs);
    glDeleteShader(fs);

    // draw one object preparation FINISHED
}

void Scene::cook_one_object_two_vbo_one_attribute_in_each()
{
    // now 2 vbo in 1 vao

    // warning - opengl porvids API which must be used in state machine manner

    // declare first vbo
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(object_sl.vertex_position_data),
                 object_sl.vertex_position_data,
                 GL_STATIC_DRAW);

    // declare second vbo
    GLuint vbo_2;

    glGenBuffers(1, &vbo_2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_2);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(object_sl.vertex_color_data),
                 object_sl.vertex_color_data,
                 GL_STATIC_DRAW);

    // declare vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // work with positions vbo as state machine had "forggoten" about what we are talking
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // desribe position attribute values
    glVertexAttribPointer(0, /* <---- */
                          object_sl.vertex_position_attribute_data.packs_arr[0].n_attributes_in_pack,
                          object_sl.vertex_position_attribute_data.packs_arr[0].data_pack.data_type,
                          GlBooleanSymbolicConstant(
                              !object_sl.vertex_position_attribute_data.packs_arr[0]
                                   .data_pack.normalized_data_flag),
                          object_sl.vertex_position_attribute_data.packs_arr[0].stride,
                          object_sl.vertex_position_attribute_data.packs_arr[0].offset);
    glEnableVertexAttribArray(0);

    // now work with colors vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_2);

    // desribe color attribute values
    glVertexAttribPointer(1, /* <---- /*/
                          object_sl.vertex_color_attribute_data.packs_arr[0].n_attributes_in_pack,
                          object_sl.vertex_color_attribute_data.packs_arr[0].data_pack.data_type,
                          GlBooleanSymbolicConstant(
                              !object_sl.vertex_color_attribute_data.packs_arr[0]
                                   .data_pack.normalized_data_flag),
                          object_sl.vertex_color_attribute_data.packs_arr[0].stride,
                          object_sl.vertex_color_attribute_data.packs_arr[0].offset);
    glEnableVertexAttribArray(1);

    // they same code with preparing shaders (for object_sl)

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,
                   1,
                   &object_sl.v_shader_src,
                   NULL); // NULL tells that strings are null-terminated
    glCompileShader(vs);

    // check for compile errors
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &object_sl.f_shader_src, NULL);
    glCompileShader(fs);

    // check
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);
    }

    // create shader program and link shaders to program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // check
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader_program_, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);
    }

    // clean up shaders, they are linked already
    glDeleteShader(vs);
    glDeleteShader(fs);

    // draw one object preparation FINISHED
}

bool Scene::getIndex_draw() const
{
    return index_draw;
}
