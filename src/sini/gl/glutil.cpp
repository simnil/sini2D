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

	GLuint loadShaderProgram(const char* vertex_shader_src,	const char* fragment_shader_src,
		void(*attrib_bind_func_ptr)(uint32_t shader_program)) noexcept {

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

		if (attrib_bind_func_ptr)
			attrib_bind_func_ptr(shader_program);

		bool link_success = linkShaderProgram(shader_program);

		glDetachShader(shader_program, vertex_shader);
		glDetachShader(shader_program, fragment_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		if (!link_success) {
			std::cerr << "Could not link shader program" << std::endl;
			glDeleteShader(shader_program);
			return 0;
		}
		return shader_program;
	}

	bool linkShaderProgram(GLuint shader_program) noexcept {

		glLinkProgram(shader_program);

		GLint link_ok;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &link_ok);
		return static_cast<bool>(link_ok);
	}

	GLuint loadShaderProgram(const char* vertex_shader_src, const char* geometry_shader_src,
		const char* fragment_shader_src,
		void(*attrib_bind_func_ptr)(uint32_t shader_program)) noexcept {

		GLuint vertex_shader = loadShader(vertex_shader_src, GL_VERTEX_SHADER);
		if (vertex_shader == 0) {
			std::cerr << "Vertex shader could not be compiled" << std::endl;
			return 0;
		}
		GLuint geometry_shader;
		bool skip_geom_shader = (geometry_shader_src == nullptr
			|| geometry_shader_src == NULL);
		if (!skip_geom_shader) {
			geometry_shader = loadShader(geometry_shader_src, GL_GEOMETRY_SHADER);
			if (geometry_shader == 0) {
				std::cerr << "Geometry shader could not be compiled" << std::endl;
				return 0;
			}
		}
		GLuint fragment_shader = loadShader(fragment_shader_src, GL_FRAGMENT_SHADER);
		if (fragment_shader == 0) {
			std::cerr << "Fragment shader could not be compiled" << std::endl;
			return 0;
		}

		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		if (!skip_geom_shader) glAttachShader(shader_program, geometry_shader);
		glAttachShader(shader_program, fragment_shader);

		if (attrib_bind_func_ptr)
			attrib_bind_func_ptr(shader_program);

		bool link_success = linkShaderProgram(shader_program);

		glDetachShader(shader_program, vertex_shader);
		glDetachShader(shader_program, fragment_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		if (!skip_geom_shader) {
			glDetachShader(shader_program, geometry_shader);
			glDeleteShader(geometry_shader);
		}

		if (!link_success) {
			std::cerr << "Could not link shader program" << std::endl;
			glDeleteShader(shader_program);
			return 0;
		}
		return shader_program;
	}

}} // namespace gl, namespace sini