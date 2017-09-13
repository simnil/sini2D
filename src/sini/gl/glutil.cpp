#include "sini/gl/glutil.hpp"

namespace sini { namespace gl {

	GLuint loadShader(const char* shader_src, uint32_t shader_type) {

		GLuint shader = glCreateShader(shader_type);
		glShaderSource(shader, 1, &shader_src, NULL);
		glCompileShader(shader);

		GLint compile_ok;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
		if (!compile_ok) {
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}

	GLuint loadShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src) noexcept {

		GLuint vertex_shader = loadShader(vertex_shader_src, GL_VERTEX_SHADER);
		if (vertex_shader == 0) {
			std::cerr << "Vertex shader could not be compiled" << std::endl;
			return 0;
		}

		GLuint fragment_shader = loadShader(fragment_shader_src, GL_FRAGMENT_SHADER);
		if (fragment_shader == 0) {
			std::cerr << "Fragment shader could not be compiled" << std::endl;
			return 0;
		}

		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return shader_program;
	}

	bool linkShaderProgram(GLuint shader_program) noexcept {

		glLinkProgram(shader_program);

		GLint link_ok;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &link_ok);
		return static_cast<bool>(link_ok);
	}

}} // namespace gl, namespace sini