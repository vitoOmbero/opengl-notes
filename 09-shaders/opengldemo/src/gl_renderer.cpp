#include "gl_renderer.h"

#include <algorithm>
#include <cassert>

#include "gl_rendering_target.h"
#include "utils/gl_enum.h"
#include "utils/terminal.h"

#define TAG "GlRenderer >>"


#define GL_CHECK()                                                 \
  {                                                                \
    const unsigned int err = glGetError();                         \
    if (err != GL_NO_ERROR) {                                      \
      switch (err) {                                               \
        case GL_INVALID_ENUM:                                      \
          Terminal::ReportErr("GL_INVALID_ENUM");                  \
          break;                                                   \
        case GL_INVALID_VALUE:                                     \
          Terminal::ReportErr("GL_INVALID_VALUE");                 \
          break;                                                   \
        case GL_INVALID_OPERATION:                                 \
          Terminal::ReportErr("GL_INVALID_OPERATION");             \
          break;                                                   \
        case GL_INVALID_FRAMEBUFFER_OPERATION:                     \
          Terminal::ReportErr("GL_INVALID_FRAMEBUFFER_OPERATION"); \
          break;                                                   \
        case GL_OUT_OF_MEMORY:                                     \
          Terminal::ReportErr("GL_OUT_OF_MEMORY");                 \
          break;                                                   \
        default:                                                   \
          Terminal::ReportErr("Unknown GL error");                 \
      }                                                            \
      assert(false);                                               \
    }                                                              \
  }

GlRenderer::GlRenderer()
{
  glClearColor(.24, .12, .12, 1.);
  GL_CHECK()
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  GL_CHECK()

  glGenVertexArrays(1, &vao_);
  GL_CHECK()
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

    for (auto mesh : ml)
    {
        ProcessMeshBoundedRenderingTargets(
            mesh.getRtpp(), mesh.getDrawingSpec(), mesh.getShaderPack());
    }
}

void GlRenderer::ProcessMeshBoundedRenderingTargets(
    const gl_rendering_target::RenderingTargetPackPointer rtpp, DrawingSpec ds,
    const gl_rendering_target::ShaderPack &sp) {
  using gl_rendering_target::AttributePackSpecification;
  using gl_rendering_target::RenderingTargetPackPointer;
  using gl_rendering_target::VertexDataPointer;

  size_t aps_i = 0;
  size_t vdp_i = 0;

#ifdef RENDERER_VERBOSE
  if (rtpp.n_aps == 0 && rtpp.n_vdp == 0) {
    Terminal::ReportErr(TAG "Warning: RenderingTargetPackPointer at address");
    Terminal::ReportErr(
        static_cast<void *>(const_cast<RenderingTargetPackPointer *>(&rtpp)));
    Terminal::ReportErr("may be ill-formed. ");

    Terminal::ReportErr("RenderingTargetPack size is zero.");
  }
#endif

  size_t aps_last = rtpp.n_aps - 1;
  size_t vdp_last = rtpp.n_vdp - 1;

#ifdef GL_DEMO_DEBUG
  if (aps_last != vdp_last && vdp_last != 0) {
    Terminal::ReportErr(TAG "Warning: RenderingTargetPackPointer at address");
    Terminal::ReportErr(
        static_cast<void *>(const_cast<RenderingTargetPackPointer *>(&rtpp)));
    Terminal::ReportErr("may be ill-formed. ");

    Terminal::ReportErr("RenderingTargetPack data is not correctly composed.");
    Terminal::ReportErr("CompositionCase::k1N needs 1 vdp only.");
  }
#endif

  if (vdp_last == 1 && aps_last == vdp_last) {
    ProcessRenderingTargetK11(&rtpp, ds, 0, sp);
  } else if (aps_last == vdp_last)
    for (unsigned char i = 0; i < rtpp.n_aps; ++i)
      ProcessRenderingTargetK11(&rtpp, ds, i, sp);
  else
    ProcessRenderingTargetK1N(&rtpp, ds, sp);

  aps_binding_counter_ += rtpp.n_aps;
}

void GlRenderer::CreateIndexedTarget(
    const gl_rendering_target::VertexDataPointer *vdp, DrawingSpec ds,
    const gl_rendering_target::ShaderPack &sp) {
  GLuint ibo;
  glGenBuffers(1, &ibo);
  GL_CHECK()
  ebo_.push_back(ibo);

  i_target_id_.push_back(ibo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  GL_CHECK()

  GLvoid *offset = reinterpret_cast<GLvoid *>(accumulative_ebo_offset_);
  i_target_offsets_.push_back(offset);
  i_target_sizes_.push_back(vdp->n_indices);

  accumulative_ebo_offset_ += SizeOfIndexData(vdp);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vdp->n_indices, vdp->i_data,
               DrawingSpecToGlenum(ds));
  GL_CHECK()

  CookShaderProgram(sp, i_target_shaders_);
}

void GlRenderer::CreateUnIndexedTarget(
    const gl_rendering_target::VertexDataPointer *vdp, DrawingSpec ds,
    const gl_rendering_target::ShaderPack &sp) {
  auto vbo = vbo_[vbo_.size() - 1];

  v_target_id_.push_back(vbo);
  v_target_sizes.push_back(vdp->n_vertices);

  CookShaderProgram(sp, v_target_shaders_);
}

void GlRenderer::CreateVbo() {
  glBindVertexArray(vao_);
  GL_CHECK()

  GLuint vbo;
  glGenBuffers(1, &vbo);
  GL_CHECK()
  vbo_.push_back(vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GL_CHECK()
}

void GlRenderer::CookShaderProgram(const gl_rendering_target::ShaderPack &sp,
                                   std::vector<GLuint> &shaders_container) {
  // create vertex shader
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  GL_CHECK()

  std::string v = sp.vertex_src;

  const GLchar *vsrc = v.data();

#ifdef RENDERER_VERBOSE
  {
    Terminal::ReportMsg(TAG "Loaded vertex shader:");
    Terminal::ReportMsg(v);
    Terminal::ReportMsg(TAG "EOF");
  }
#endif

  glShaderSource(vs, 1, &vsrc, nullptr);
  GL_CHECK()
  glCompileShader(vs);
  GL_CHECK()

  // check for compile errors
  GLint result;
  GLchar infoLog[1024];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
  GL_CHECK()
  if (!result) {
    glGetShaderInfoLog(vs, sizeof(infoLog), nullptr, infoLog);
    GL_CHECK()
    Terminal::ReportErr("Error! Vertex shader failed to compile. ");
    Terminal::ReportErr(infoLog);

    return;
  }

  // create fragment shader
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  GL_CHECK()

  v = sp.fragment_src;
  const GLchar *fsrc = v.data();

#ifdef RENDERER_VERBOSE
  {
    Terminal::ReportMsg(TAG "Loaded fragment shader:");
    Terminal::ReportMsg(v);
    Terminal::ReportMsg(TAG "EOF");
  }
#endif

  glShaderSource(fs, 1, &fsrc, NULL);
  GL_CHECK()
  glCompileShader(fs);
  GL_CHECK()

  // check
  glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
    Terminal::ReportErr("Error! Fragment shader failed to compile. ");
    Terminal::ReportErr(infoLog);

    return;
  }

  // create shader program and link shaders to program
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vs);
  GL_CHECK()
  glAttachShader(shader_program, fs);
  GL_CHECK()
  glLinkProgram(shader_program);
  GL_CHECK()

  // check
  glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
  GL_CHECK()
  if (!result) {
    glGetProgramInfoLog(shader_program, sizeof(infoLog), NULL, infoLog);
    Terminal::ReportErr("Error! Shader program linker failure ");
    Terminal::ReportErr(infoLog);

    return;
  }

  // clean up shaders, they are linked already
  glDeleteShader(vs);
  GL_CHECK()
  glDeleteShader(fs);
  GL_CHECK()

  // save program
  shaders_container.emplace_back(shader_program);
}

void GlRenderer::ProcessRenderingTargetK11(
    const gl_rendering_target::RenderingTargetPackPointer *rtpp, DrawingSpec ds,
    unsigned char target_index, const gl_rendering_target::ShaderPack &sp) {
  using namespace gl_rendering_target;

#ifdef RENDERER_VERBOSE
  Terminal::ReportMsg(TAG "Processing case k11");
#endif
  CreateVbo();
  glBufferData(GL_ARRAY_BUFFER,
               gl_rendering_target::SizeOfVertexData(*rtpp, target_index),
               rtpp->vdp[target_index].v_data, DrawingSpecToGlenum(ds));
  GL_CHECK()
#ifdef RENDERER_VERBOSE
  if (rtpp->aps[target_index].layout != AttributeDataLayout::kContiguous) {
    Terminal::ReportErr(TAG "Warning: VertexAttributeData at address");
    Terminal::ReportErr(
        static_cast<void *>(const_cast<RenderingTargetPackPointer *>(rtpp)));
    Terminal::ReportErr("may be ill-formed. ");

    Terminal::ReportErr(TAG "AttributeDataLayout should be: ");
    Terminal::ReportErr(std::to_string((int)AttributeDataLayout::kContiguous));
    Terminal::ReportErr("is: ");
    Terminal::ReportErr(std::to_string((int)rtpp->aps[target_index].layout));
  }
#endif


  glVertexAttribPointer(
      aps_binding_counter_,
      attribute::RuntimeMap::Get(rtpp->aps[target_index].scheme)->attributes_n,
      rtpp->aps[target_index].type_code,
      BooleanToGLbool(rtpp->aps[target_index].is_data_NOT_normalized),
      rtpp->aps[target_index].stride, rtpp->aps[target_index].offset);
  GL_CHECK()

  glEnableVertexAttribArray(aps_binding_counter_);
  GL_CHECK()

  if (rtpp->vdp[target_index].i_data != nullptr) {
    CreateIndexedTarget(&rtpp->vdp[target_index], ds, sp);
  } else {
    CreateUnIndexedTarget(&rtpp->vdp[target_index], ds, sp);
  }
}

void GlRenderer::ProcessRenderingTargetK1N(
    const gl_rendering_target::RenderingTargetPackPointer *rtpp, DrawingSpec ds,
    const gl_rendering_target::ShaderPack &sp) {
#ifdef RENDERER_VERBOSE
  Terminal::ReportMsg(TAG "Processing case k1N");
#endif

  CreateVbo();

  auto s = gl_rendering_target::SizeOfVertexData(*rtpp, 0);
  auto d = rtpp->vdp[0].v_data;
  auto ddss = DrawingSpecToGlenum(ds);

  glBufferData(GL_ARRAY_BUFFER, s, d, ddss);
  GL_CHECK()

#ifdef RENDERER_VERBOSE
  if (rtpp->aps[0].layout !=
      gl_rendering_target::AttributeDataLayout::kInterleaved) {
    Terminal::ReportErr(TAG "Warning: RenderingTargetPackPointer at adress");
    Terminal::ReportErr(static_cast<void *>(
        const_cast<gl_rendering_target::RenderingTargetPackPointer *>(rtpp)));
    Terminal::ReportErr("may be ill-formed. ");

    Terminal::ReportErr(TAG "VertexAttributeData::CompositionCase should be: ");
    Terminal::ReportErr(std::to_string(
        (int)gl_rendering_target::AttributeDataLayout::kInterleaved));
    Terminal::ReportErr("is: ");
    Terminal::ReportErr(std::to_string((int)rtpp->aps[0].layout));
  }
#endif

  glBindVertexArray(vao_);
  GL_CHECK()

  for (unsigned char i = 0; i < rtpp->n_aps; ++i) {
    auto name = aps_binding_counter_ + i;
    auto number = attribute::RuntimeMap::Get(rtpp->aps[i].scheme)->attributes_n;
    auto gltype = rtpp->aps[i].type_code;
    auto glbool = BooleanToGLbool(rtpp->aps[i].is_data_NOT_normalized);
    auto stride = rtpp->aps[i].stride;
    auto offset = rtpp->aps[i].offset;

    glVertexAttribPointer(name, number, gltype, glbool, stride, offset);
    GL_CHECK()

    glEnableVertexAttribArray(aps_binding_counter_ + i);
    GL_CHECK()
  }



  if (rtpp->vdp[0].i_data != nullptr) {
    CreateIndexedTarget(&rtpp->vdp[0], ds, sp);
  } else {
    CreateUnIndexedTarget(&rtpp->vdp[0], ds, sp);
  }
}

void GlRenderer::RenderFrame() {
#ifdef RENDERER_VERBOSE
  static bool already_reported = false;

  if (!already_reported)
    if (vbo_.size() == 0 || v_target_shaders_.size() == 0) {
      if (already_reported) return;

      Terminal::ReportErr("GlRenderer is not initialized.");
      already_reported = true;
      return;
    }
  already_reported = true;
#endif

  glClear(GL_COLOR_BUFFER_BIT);
  GL_CHECK()

  // Render targets is working only with one vao (currently)

  // vertex targets rendering
  glBindVertexArray(vao_);
  GL_CHECK()


  for (size_t i = 0; i < v_target_sizes.size(); ++i) {
    glUseProgram(v_target_shaders_[i]);
    GL_CHECK()
    glDrawArrays(GL_TRIANGLES, 0, v_target_sizes[i]);
    GL_CHECK()
  }

  // NOTE: not done yet
  // indexed targets rendering

  if (ebo_.size() > 0)
    for (size_t i = 0; i < i_target_sizes_.size(); ++i) {
      glUseProgram(i_target_shaders_[i]);
      GL_CHECK()
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_[i]);
      GL_CHECK()
      glDrawElements(GL_TRIANGLES, i_target_sizes_[i], GL_UNSIGNED_INT,
                     i_target_offsets_[i]);
      GL_CHECK()
    }

  glBindVertexArray(0);
  GL_CHECK()
}

GlRenderer::~GlRenderer() {
  for (auto i : v_target_shaders_) glDeleteProgram(i);
  for (auto i : i_target_shaders_) glDeleteProgram(i);
  for (auto i : vbo_) glDeleteBuffers(1, &i);
  for (auto i : ebo_) glDeleteBuffers(1, &i);
  glDeleteVertexArrays(1, &vao_);
}