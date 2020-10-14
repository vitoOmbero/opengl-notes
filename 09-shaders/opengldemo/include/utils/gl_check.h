#ifndef GL_CHECK_H
#define GL_CHECK_H

#include <cassert>
#define GL_CHECK()                                                             \
    {                                                                          \
        const unsigned int err = glGetError();                                 \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    Terminal::ReportErr("GL_INVALID_ENUM");                    \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    Terminal::ReportErr("GL_INVALID_VALUE");                   \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    Terminal::ReportErr("GL_INVALID_OPERATION");               \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    Terminal::ReportErr("GL_INVALID_FRAMEBUFFER_OPERATION");   \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    Terminal::ReportErr("GL_OUT_OF_MEMORY");                   \
                    break;                                                     \
            }                                                                  \
            assert(false);                                                     \
        }                                                                      \
    }

#endif // GL_CHECK_H
