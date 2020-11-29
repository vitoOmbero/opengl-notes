#ifndef MESH_H
#define MESH_H

#include <string>

#include "gl_rendering_target.h"
#include "utils/gl_enum.h"

// NOTE: meshes do not support shared shaders yet

class Mesh
{
public:
    Mesh(gl_rendering_target::RenderingTargetPackPointer* rtpp_,
         std::string vertex_shader_src, std::string fragment_shader_src,
         DrawingSpec drawing_spec = DrawingSpec::kStatic);

    // NOTE: no, they won't be default all the time
    Mesh(const Mesh& another);
    Mesh& operator=(const Mesh& another);
    Mesh(Mesh&& another) noexcept ;
    Mesh& operator=(Mesh&& another) noexcept ;

    [[nodiscard]] DrawingSpec getDrawingSpec() const;
    void        setDescriptor(const DrawingSpec& descriptor);

    [[nodiscard]] std::string getFragmentShaderSrc() const;

    [[nodiscard]] std::string getVertexShaderSrc() const;

    [[nodiscard]] gl_rendering_target::RenderingTargetPackPointer getRtpp() const;
    void setRtpp(const gl_rendering_target::RenderingTargetPackPointer& value);

    gl_rendering_target::ShaderPack getShaderPack() const;
    void       setShaderPack(const gl_rendering_target::ShaderPack& shader_pack);

private:
#pragma pack(push, 4)
    gl_rendering_target::RenderingTargetPackPointer rtpp_;
    std::string                                     v_shader_src_;
    std::string          f_shader_src_;
    DrawingSpec          drawing_spec_;
    gl_rendering_target::ShaderPack                                      shader_pack_;
#pragma pack(pop)
};

template <class T>
struct is_mesh
{
    static const bool value = false;
};

template <>
struct is_mesh<Mesh>
{
    static const bool value = true;
};
#endif // MESH_H
