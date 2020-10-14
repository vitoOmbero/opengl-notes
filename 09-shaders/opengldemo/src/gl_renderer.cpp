#include "gl_renderer.h"

#include "utils/gl_enum.h"
#include "utils/terminal.h"

#define TAG "GlRenderer >>"

static char kAttributesMax{ 8 };

GlRenderer::GlRenderer()
    : vbo_()
    , ebo_()
    , shaders_()
    , target_sizes()
    , vao_binding_counter_{ 0 }
{
    glGenVertexArrays(1, &vao_);

    glClearColor(.24, .12, .24, 1.);
}

void GlRenderer::Init(MeshLoader& ml)
{
    // NOTE: All OpenGL ES 2.0 implementations must support a minimum of eight
    // vertex attributes.

#ifdef RENDERER_VERBOSE
    {
        GLint maxVertexAttribs;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
        Terminal::ReportMsg(TAG "maxVertexAttribs: ");
        Terminal::ReportMsg(std::to_string(maxVertexAttribs));
    }
#endif

    for (auto i : ml)
    {
        // TODO: extract invariant
#ifdef RENDERER_VERBOSE
        static int cnt{ 0 };
        {
            if (i.getVertexAttributeData()->vap->stride == 0 &&
                i.getVertexAttributeData()->adp->layout == Layout::kInterleaved)
            {
                Terminal::ReportErr(
                    TAG " Warning: ill-formed VertexAttributeData in mesh ");
                Terminal::ReportErr(std::to_string(cnt));
            }
        }
#endif

        GLuint cnt_prev = vao_binding_counter_;

        auto n_vbo = CreateVbo(i);

        if (n_vbo > 1)
        {
            Terminal::ReportErr(TAG " Warning: mesh ");
            Terminal::ReportErr(std::to_string(cnt));
            Terminal::ReportErr(" uses ");
            Terminal::ReportErr(std::to_string(n_vbo));
            Terminal::ReportErr("VBOs");
        }

        glBindVertexArray(vao_);

        BindVbo(i, n_vbo, cnt_prev);

        CookShaderProgram(i);

        // NOTE: indces are not supported yet
        target_sizes.emplace_back(i.getVertexAttributeData()->adp->n_vertices);
    }
}

size_t GlRenderer::CreateVbo(const Mesh& mesh)
{
    auto create = [](std::vector<GLuint>& v) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        v.push_back(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    };

    if (mesh.getVertexAttributeData()->adp->layout == Layout::kInterleaved)
    {
        create(vbo_);

        glBufferData(GL_ARRAY_BUFFER, mesh.getSizeOfDataPack(),
                     mesh.getVertexAttributeData()->adp->data,
                     DrawingSpecToGlenum(mesh.getDrawingSpec()));

        return 1;
    }

    if (mesh.getVertexAttributeData()->adp->layout == Layout::kContiguous)
    {
        for (size_t i{ 0 };
             i < mesh.getVertexAttributeData()->n_vertex_attribute_packs; ++i)
        {
            create(vbo_);
            glBufferData(GL_ARRAY_BUFFER, mesh.getSizeOfDataPack(i),
                         mesh.getVertexAttributeData()->adp->data,
                         DrawingSpecToGlenum(mesh.getDrawingSpec()));
        }
        return mesh.getVertexAttributeData()->n_vertex_attribute_packs;
    }

    Terminal::ReportErr(TAG "current Layout for attributes is not supported.");
    return 0;
}

void GlRenderer::BindVbo(const Mesh& mesh, size_t vbo_count,
                         GLuint vao_counter_before_vbo_creation)
{
    // inner cycle works if only there were created > 1 vbo

    GLuint j = 0;

    for (auto i = vao_counter_before_vbo_creation; i < vbo_count; ++i)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[i]);

        for (;; ++j)
        {
            auto vap = mesh.getVertexAttributeData()->vap[j];

            glVertexAttribPointer(
                i, vap.n_attributes_in_pack, vap.data_pack.data_type,
                BooleanToGLbool(!vap.data_pack.normalized_data_flag),
                vap.stride, vap.offset);

            if (vbo_count > 1)
            {
                if (j < mesh.getVertexAttributeData()->n_attribute_data_packs)
                {
                    ++j;
                    break;
                }
                else
                    break;
            };
            break;
        }

        glEnableVertexAttribArray(i);
    }
}

void GlRenderer::CookShaderProgram(const Mesh& mesh)
{
    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);

    std::string v = mesh.getVertexShaderSrc();

    const GLchar* vsrc = v.data();

    glShaderSource(vs, 1, &vsrc, NULL);
    // GL_CHECK()
    glCompileShader(vs);
    // GL_CHECK()

    // check for compile errors
    GLint  result;
    GLchar infoLog[1024];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Vertex shader failed to compile. ");
        Terminal::ReportErr(infoLog);

        return;
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    v                  = mesh.getFragmentShaderSrc();
    const GLchar* fsrc = v.data();

    glShaderSource(fs, 1, &fsrc, NULL);
    glCompileShader(fs);

    // check
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Fragment shader failed to compile. ");
        Terminal::ReportErr(infoLog);

        return;
    }

    // create shader program and link shaders to program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    // check
    glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader_program, sizeof(infoLog), NULL, infoLog);
        Terminal::ReportErr("Error! Shader program linker failure ");
        Terminal::ReportErr(infoLog);

        return;
    }

    // clean up shaders, they are linked already
    glDeleteShader(vs);
    glDeleteShader(fs);

    // save program

    shaders_.emplace_back(shader_program);
}

void GlRenderer::RenderFrame()
{
    if (vbo_.size() == 0 || shaders_.size() == 0)
    {
        static bool already_reported = false;

        if (already_reported)
            return;

        Terminal::ReportErr("GlRenderer is not initialized.");
        already_reported = true;
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    // Render targets
    glBindVertexArray(vao_);

    for (size_t i = 0; i < target_sizes.size(); ++i)
    {
        glUseProgram(shaders_[i]);
        glDrawArrays(GL_TRIANGLES, 0, target_sizes[i]);
    }
    // NOTE: not done yet
    //    glDrawElements(GL_TRIANGLES, n_indices_, GL_UNSIGNED_INT, 0); // 0
    //    for offset
    glBindVertexArray(0);
}

GlRenderer::~GlRenderer()
{
    for (auto i : shaders_)
        glDeleteProgram(i);
    for (auto i : vbo_)
        glDeleteBuffers(1, &i);
    for (auto i : ebo_)
        glDeleteBuffers(1, &i);
    glDeleteVertexArrays(1, &vao_);
}
