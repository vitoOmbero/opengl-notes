#ifndef GL_ENUM_H
#define GL_ENUM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class DrawingSpec
{
    kStatic,
    kDynamic,
    kStreaming
};

inline static GLenum DrawingSpecToGlenum(DrawingSpec ds)
{
    switch (ds)
    {
        case (DrawingSpec::kStatic):
            return GL_STATIC_DRAW;
        case (DrawingSpec::kDynamic):
            return GL_DYNAMIC_DRAW;
        case (DrawingSpec::kStreaming):
            return GL_STREAM_DRAW;
        default:
            return 0;
    }
}

/**
 * @brief SizeOfGlTypeBySymbolicConstant
 * @param gl_symb_constant GL_FLOAT, GL_FIXED, etc
 * @return sizeof(gl_associated_type) or 0
 */
inline static size_t SizeOfGlTypeByGLenum(GLenum gl_symb_constant)
{
    switch (gl_symb_constant)
    {
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

inline GLboolean BooleanToGLbool(bool arg)
{
    return arg ? GL_TRUE : GL_FALSE;
}

#endif // GL_ENUM_H
