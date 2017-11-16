#pragma once
#ifndef SINI_GL_UTIL_H
#define SINI_GL_UTIL_H

#include "GL/glew.h"
#include <iostream>
#include <string>

namespace sini { namespace gl {

// Load and compile a single shader
GLuint loadShader(const char* shader_src, uint32_t shader_type);

// Link a previously loaded shader program
bool linkShaderProgram(GLuint shader_program) noexcept;

// Compile and load a vertex and fragment shader
// If attribute locations aren't explicitly bound in the shader source code
// a pointer to a function binding them can be provided
GLuint loadShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src,
    void(*attrib_bind_func_ptr)(uint32_t shader_program) = nullptr) noexcept;

// Compile and load a vertex, geometry and fragment shader
// If attribute locations aren't explicitly bound in the shader source code
// a pointer to a function binding them can be provided
GLuint loadShaderProgram(const char* vertex_shader_src, const char* geomentry_shader_src,
    const char* fragment_shader_src,
    void(*attrib_bind_func_ptr)(uint32_t shader_program) = nullptr) noexcept;

}} // namespace gl, namespace sini
#endif // SINI_GL_UTIL_H
