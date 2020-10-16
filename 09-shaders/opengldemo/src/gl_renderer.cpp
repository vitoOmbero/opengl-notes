#include "gl_renderer.h"

#include <cassert>

#include "gl_rendering_target.h"
#include "utils/gl_enum.h"
#include "utils/terminal.h"

#define TAG "GlRenderer >>"

GlRenderer::GlRenderer()
    : vbo_()
    , ebo_()
    , shaders_()
    , target_sizes()
    , vap_binding_counter_{ 0 }
{
    glGenVertexArrays(1, &vao_);

    glClearColor(.24, .12, .24, 1.);
}

void GlRenderer::Init(MeshLoader& ml)
{

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
                i.getVertexAttributeData()->adp->layout ==
                    AttributeDataLayout::kInterleaved)
            {
                Terminal::ReportErr(
                    TAG " Warning: ill-formed VertexAttributeData in mesh ");
                Terminal::ReportErr(std::to_string(cnt));
            }
        }
#endif

        CreateAndBindVertexBufferObjects(vao_, i.getVertexAttributeData(),
                                         i.getDrawingSpec());

        CookShaderProgram(i);

        // NOTE: indces are not supported yet
        target_sizes.emplace_back(i.getVertexAttributeData()->adp->n_vertices);
    }
}

void GlRenderer::CreateAndBindVertexBufferObjects(
    const GLuint& vao_name, const VertexAttributeData* vad, DrawingSpec ds)
{
    auto vap_counter_before_vbo_creation = vap_binding_counter_;

    auto create = [](std::vector<GLuint>& v) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        v.push_back(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    };

    auto is_shared_adp = [](const AttributeDataPack* adp) {
        return adp->shared_adp__indices_usage_flags > 0;
    };

    auto check_bit = [](unsigned char number, char bit_1_based_index) {
        return (number & (1 << (bit_1_based_index - 1))) > 0;
    };

    auto ProcessCase_k11 = [&create, &is_shared_adp,
                            &vap_counter_before_vbo_creation](
                               const GLuint&              vao_name,
                               const VertexAttributeData* vad, DrawingSpec ds,
                               std::vector<GLuint>& vbo_v,
                               unsigned char        target_adp_index,
                               unsigned char        target_vap_index) {
        create(vbo_v);
        glBufferData(GL_ARRAY_BUFFER, SizeOfDataPack(vad, target_adp_index),
                     vad->adp->data, DrawingSpecToGlenum(ds));
#ifdef RENDERER_VERBOSE
        if (is_shared_adp(&vad->adp[target_adp_index]) ||
            vad->adp[target_adp_index].layout !=
                AttributeDataLayout::kContiguous)
        {
            Terminal::ReportErr(TAG "Warning: VertexAttributeData at adress");
            Terminal::ReportErr(
                static_cast<void*>(const_cast<VertexAttributeData*>(vad)));
            Terminal::ReportErr("may be ill-formed. ");

            Terminal::ReportErr(TAG "Attribute Data Pack shared should be: ");
            Terminal::ReportErr(
                std::to_string(!is_shared_adp(&vad->adp[target_adp_index])));
            Terminal::ReportErr("is: ");
            Terminal::ReportErr(
                std::to_string(is_shared_adp(&vad->adp[target_adp_index])));

            Terminal::ReportErr(
                TAG "VertexAttributeData::CompositionCase should be: ");
            Terminal::ReportErr(
                std::to_string((int)AttributeDataLayout::kContiguous));
            Terminal::ReportErr("is: ");
            Terminal::ReportErr(
                std::to_string((int)vad->adp[target_adp_index].layout));
        }
#endif

        glBindVertexArray(vao_name);

        glVertexAttribPointer(
            vap_counter_before_vbo_creation,
            attribute::RuntimeMap::Get(vad->vap_scheme[target_vap_index])
                ->attributes_n,
            vad->vap[target_vap_index].data_type,
            BooleanToGLbool(
                vad->adp[target_adp_index].is_NOT_data_normalized_flag),
            vad->vap[target_vap_index].stride,
            vad->vap[target_vap_index].offset);

        glEnableVertexAttribArray(vap_counter_before_vbo_creation);
    };

    auto ProcessCase_k1N = [&create, &is_shared_adp,
                            &vap_counter_before_vbo_creation](
                               const GLuint&              vao_name,
                               const VertexAttributeData* vad, DrawingSpec ds,
                               std::vector<GLuint>& vbo_v,
                               unsigned char        target_adp_index,
                               unsigned char        target_vap_start_index,
                               bool excluded_vap_i[kVapSchemeArrSize] = nullptr,
                               unsigned char shared_adp_index         = 0) {
        create(vbo_v);
        for (unsigned char i = 0; i < vad->n_vertex_attribute_packs; ++i)
        {
            glBufferData(GL_ARRAY_BUFFER, SizeOfDataPack(vad, i),
                         vad->adp[target_adp_index].data,
                         DrawingSpecToGlenum(ds));
        }

#ifdef RENDERER_VERBOSE
        if (is_shared_adp(&vad->adp[target_adp_index]) ||
            vad->adp[target_adp_index].layout !=
                AttributeDataLayout::kInterleaved)
        {
            Terminal::ReportErr(TAG "Warning: VertexAttributeData at adress");
            Terminal::ReportErr(
                static_cast<void*>(const_cast<VertexAttributeData*>(vad)));
            Terminal::ReportErr("may be ill-formed. ");

            Terminal::ReportErr(TAG "Attribute Data Pack shared should be: ");
            Terminal::ReportErr(
                std::to_string(!is_shared_adp(&vad->adp[target_adp_index])));
            Terminal::ReportErr("is: ");
            Terminal::ReportErr(
                std::to_string(is_shared_adp(&vad->adp[target_adp_index])));

            Terminal::ReportErr(
                TAG "VertexAttributeData::CompositionCase should be: ");
            Terminal::ReportErr(
                std::to_string((int)AttributeDataLayout::kInterleaved));
            Terminal::ReportErr("is: ");
            Terminal::ReportErr(
                std::to_string((int)vad->adp[target_adp_index].layout));
        }
#endif

        glBindVertexArray(vao_name);

        for (unsigned char cur_vap = target_vap_start_index;
             cur_vap < vad->n_vertex_attribute_packs; ++cur_vap)
        {
            if (excluded_vap_i != nullptr)
            {
                if (excluded_vap_i[cur_vap - shared_adp_index])
                {
                    continue;
                }
            }

            glVertexAttribPointer(
                vap_counter_before_vbo_creation + cur_vap,
                attribute::RuntimeMap::Get(vad->vap_scheme[cur_vap])
                    ->attributes_n,
                vad->vap[cur_vap].data_type,
                BooleanToGLbool(
                    vad->adp[target_adp_index].is_NOT_data_normalized_flag),
                vad->vap[cur_vap].stride, vad->vap[cur_vap].offset);

            glEnableVertexAttribArray(vap_counter_before_vbo_creation +
                                      cur_vap);
        }
    };

    if (vad->composition == VertexAttributeData::CompositionCase::k11)
    {
        ProcessCase_k11(vao_name, vad, ds, vbo_, 0, 0);
        ++vap_binding_counter_;
        return;
    }

    if (vad->composition == VertexAttributeData::CompositionCase::k1N)
    {

        ProcessCase_k1N(vao_name, vad, ds, vbo_, 0, 0);
        vap_binding_counter_ += vad->n_vertex_attribute_packs;

        return;
    }

    if (vad->composition == VertexAttributeData::CompositionCase::kArr)
    {

        if (vad->n_attribute_data_packs == 1)
        {
            Terminal::ReportErr(TAG "Error: VertexAttributeData at adress");
            Terminal::ReportErr(
                static_cast<void*>(const_cast<VertexAttributeData*>(vad)));
            Terminal::ReportErr("may be ill-formed. ");

            Terminal::ReportErr(TAG "Attribute Data Pack number should be > 1");
        }

        unsigned char cur_adp = 0;
        unsigned char cur_vap = 0;

        bool          excluded_vap_i[kVapSchemeArrSize];
        static bool   was_shared_adp = false;
        unsigned char shared_adp_i   = 0;

        for (; cur_adp < vad->n_attribute_data_packs;)
        {
            if (is_shared_adp(&(vad->adp[cur_adp])))
            {
                if (was_shared_adp)
                {
                    Terminal::ReportErr(TAG
                                        "Error: VertexAttributeData at adress");
                    Terminal::ReportErr(static_cast<void*>(
                        const_cast<VertexAttributeData*>(vad)));
                    Terminal::ReportErr("may be ill-formed. ");

                    Terminal::ReportErr(
                        TAG
                        "Number of Shared Attribute Data Packs should be == 1");
                    return;
                }

                was_shared_adp = true;
                shared_adp_i   = cur_adp;

                for (int i = 1; i < 9; ++i)
                {
                    excluded_vap_i[i - 1] = check_bit(
                        vad->adp[cur_adp].shared_adp__indices_usage_flags, i);
                }

                for (int i = 0; i < 8; ++i)
                {
                    if (excluded_vap_i[i])
                    {
                        ProcessCase_k11(vao_name, vad, ds, vbo_, shared_adp_i,
                                        cur_vap + i);
                        ++vap_binding_counter_;
                    }
                }

                ++cur_adp;
                continue;
            }

            for (; cur_vap < vad->n_vertex_attribute_packs;)
            {
                if (excluded_vap_i[cur_vap])
                {
                    ++cur_vap;
                    continue;
                }

                if (vad->adp[cur_adp].associated_vap_n == 0)
                {
                    Terminal::ReportErr(TAG
                                        "Error: VertexAttributeData at adress");
                    Terminal::ReportErr(static_cast<void*>(
                        const_cast<VertexAttributeData*>(vad)));
                    Terminal::ReportErr("may be ill-formed. ");

                    Terminal::ReportErr(TAG "Attribute Data Pack with index:");
                    Terminal::ReportErr(std::to_string(cur_adp));
                    break;
                }

                if (vad->adp[cur_adp].associated_vap_n == 1)
                {
                    ProcessCase_k11(vao_name, vad, ds, vbo_, cur_adp, cur_vap);
                    ++vap_binding_counter_;
                }

                if (vad->adp[cur_adp].associated_vap_n > 1)
                {
                    ProcessCase_k1N(vao_name, vad, ds, vbo_, cur_adp, cur_vap,
                                    excluded_vap_i, shared_adp_i);

                    int cnt{ 0 };
                    for (unsigned char i = 0; i < kVapSchemeArrSize; ++i)
                    {
                        if (excluded_vap_i[i])
                            ++cnt;
                    }

                    vap_binding_counter_ +=
                        (vad->n_vertex_attribute_packs - cnt);
                }

                ++cur_vap;
            }

            ++cur_adp;
        }
    }
}

void GlRenderer::CookShaderProgram(const Mesh& mesh)
{
    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);

    std::string v = mesh.getVertexShaderSrc();

    const GLchar* vsrc = v.data();

#ifdef RENDERER_VERBOSE
    {
        Terminal::ReportMsg(TAG "Loaded vertex shader:");
        Terminal::ReportMsg(v);
        Terminal::ReportMsg(TAG "EOF");
    }
#endif

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

#ifdef RENDERER_VERBOSE
    {
        Terminal::ReportMsg(TAG "Loaded fragment shader:");
        Terminal::ReportMsg(v);
        Terminal::ReportMsg(TAG "EOF");
    }
#endif

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
