#pragma once
#ifndef SINI_GL_UTIL_H
#define SINI_GL_UTIL_H

#include "GL/glew.h"
#include <iostream>
#include <string>

namespace sini { namespace gl {

	// Load and compile a single shader
	GLuint loadShader(const char* shader_src, uint32_t shader_type);

	// Load and compile a vertex and fragment shader
	// The shader program is not linked, as attribute locations need to be bound
	// beforehand. Linking is done with linkShaderProgram().
	GLuint loadShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src) noexcept;

	// Link a shader program previously loaded with loadShaderProgram()
	bool linkShaderProgram(GLuint shader_program) noexcept;

}} // namespace gl, namespace sini

#endif // !SINI_GL_UTIL_H