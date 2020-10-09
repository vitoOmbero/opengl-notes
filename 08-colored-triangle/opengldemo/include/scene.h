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

    DemoTriangleObject demo_triangle;
    void cook_one_object_one_vbo_one_attribute();

    TriangleObjectInterleavedLayout object_il;
    void cook_one_object_one_vbo_two_attributes();

    TriangleObjectSerializedLayout object_sl;
    void cook_one_object_two_vbo_one_attribute_in_each();
};

#endif // SCENE_H
