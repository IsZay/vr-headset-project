// cube_renderer.hpp
#pragma once

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <SDL2/SDL.h>
#include <array>

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();

    bool init();
    void draw(float roll, float pitch, float yaw);

private:
    GLuint program;
    GLuint vbo, ibo;
    GLint positionAttrib, modelUniform, projUniform;

    std::array<float, 16> projectionMatrix();
    std::array<float, 16> rotationMatrix(float roll, float pitch, float yaw);

    GLuint loadShader(GLenum type, const char* src);
    GLuint createProgram(const char* vsSrc, const char* fsSrc);
};
