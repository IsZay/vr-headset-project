#include "cube_renderer.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <cmath>
#include <iostream>

namespace {
const char* vertexShaderSource = R"(
attribute vec3 aPos;
uniform mat4 uModel;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
})";

const char* fragmentShaderSource = R"(
void main() {
    gl_FragColor = vec4(0.2, 0.8, 1.0, 1.0);
})";

const float cubeVertices[] = {
    -1, -1, -1,   1, -1, -1,
     1,  1, -1,  -1,  1, -1,
    -1, -1,  1,   1, -1,  1,
     1,  1,  1,  -1,  1,  1,
};

const GLushort cubeIndices[] = {
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 1, 5, 2, 6, 3, 7
};

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader Compilation Error: " << log << std::endl;
    }
    return shader;
}

GLuint createProgram() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void multiplyMatrix(float out[16], const float a[16], const float b[16]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out[i * 4 + j] = 0;
            for (int k = 0; k < 4; ++k)
                out[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
        }
    }
}

}  // namespace

void CubeRenderer::initialize() {
    shaderProgram = createProgram();
    aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    uModelLocation = glGetUniformLocation(shaderProgram, "uModel");
    uProjectionLocation = glGetUniformLocation(shaderProgram, "uProjection");

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);

    float aspect = 1920.0f / 1080.0f;
    float fov = 60.0f * 3.14159f / 180.0f;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tan(fov / 2);
    float proj[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far+near)/(near-far), (2*far*near)/(near-far),
        0, 0, -1, 0
    };
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(uProjectionLocation, 1, GL_FALSE, proj);
}

void CubeRenderer::draw(float roll, float pitch, float yaw) {
    float cr = cos(roll), sr = sin(roll);
    float cp = cos(pitch), sp = sin(pitch);
    float cy = cos(yaw), sy = sin(yaw);

    float rotX[16] = {
        1, 0, 0, 0,
        0, cr, -sr, 0,
        0, sr, cr, 0,
        0, 0, 0, 1
    };
    float rotY[16] = {
        cp, 0, sp, 0,
        0, 1, 0, 0,
        -sp, 0, cp, 0,
        0, 0, 0, 1
    };
    float rotZ[16] = {
        cy, -sy, 0, 0,
        sy, cy, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float modelXY[16];
    float model[16];
    multiplyMatrix(modelXY, rotY, rotX);
    multiplyMatrix(model, rotZ, modelXY);
    model[14] = -5.0f;  // Translate z

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(uModelLocation, 1, GL_FALSE, model);

    glEnableVertexAttribArray(aPosLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(aPosLocation);
}
