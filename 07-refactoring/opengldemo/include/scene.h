#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assets.h"

class Scene
{
public:
    Scene();

    GLuint getVbo() const;

    GLuint getVao() const;

    GLint getShaderProgram() const;

private:
    GLuint vbo_;
    GLuint vao_;
    GLint shader_program_;
    DemoTriangleObject object;
};

#endif // SCENE_H
