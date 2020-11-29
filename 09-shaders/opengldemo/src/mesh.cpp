#include "mesh.h"

#include <algorithm>

#include "attribute.h"

Mesh::Mesh(gl_rendering_target::RenderingTargetPackPointer* rtpp,
           std::string vertex_shader_src, std::string fragment_shader_src,
           DrawingSpec drawing_spec)
        : rtpp_{*rtpp}, drawing_spec_{drawing_spec} {
    // TODO: auto detect if indices are needed

    v_shader_src_ = vertex_shader_src;
    f_shader_src_ = fragment_shader_src;
    shader_pack_ = {vertex_shader_src, fragment_shader_src};
}

Mesh::Mesh(const Mesh& another)
        : rtpp_{another.rtpp_},
          v_shader_src_{another.v_shader_src_},
          f_shader_src_{another.f_shader_src_},
          drawing_spec_{another.drawing_spec_} {}

Mesh& Mesh::operator=(const Mesh& another) {
    // TODO: where is the resourse magament?
    // NOTE: why not default? is it final version?

    this->rtpp_ = another.rtpp_;
    this->v_shader_src_ = another.v_shader_src_;
    this->f_shader_src_ = another.f_shader_src_;
    this->drawing_spec_ = another.drawing_spec_;
    this->shader_pack_ = another.shader_pack_;
    return *this;
}

// NOTE:     how about to really free resourses?
Mesh::Mesh(Mesh&& another) noexcept
        : rtpp_{another.rtpp_},
          v_shader_src_{another.v_shader_src_},
          f_shader_src_{another.f_shader_src_},
          drawing_spec_{another.drawing_spec_},
          shader_pack_{another.shader_pack_} {}

Mesh& Mesh::operator=(Mesh&& another) noexcept {
    if (this != &another) {
        this->rtpp_ = another.rtpp_;
        this->v_shader_src_ = std::move(another.v_shader_src_);
        this->f_shader_src_ = std::move(another.f_shader_src_);
        this->drawing_spec_ = std::move(another.drawing_spec_);
        this->shader_pack_ = std::move(another.shader_pack_);
    }
    return *this;
}

DrawingSpec Mesh::getDrawingSpec() const { return drawing_spec_; }

void Mesh::setDescriptor(const DrawingSpec& descriptor) {
    drawing_spec_ = descriptor;
}

std::string Mesh::getFragmentShaderSrc() const { return f_shader_src_; }

std::string Mesh::getVertexShaderSrc() const { return v_shader_src_; }

gl_rendering_target::RenderingTargetPackPointer Mesh::getRtpp() const {
    return rtpp_;
}

void Mesh::setRtpp(
        const gl_rendering_target::RenderingTargetPackPointer& value) {
    rtpp_ = value;
}

gl_rendering_target::ShaderPack Mesh::getShaderPack() const {
    return gl_rendering_target::ShaderPack{v_shader_src_, f_shader_src_};
}

void Mesh::setShaderPack(const gl_rendering_target::ShaderPack& shader_pack) {
    shader_pack_ = shader_pack;
}