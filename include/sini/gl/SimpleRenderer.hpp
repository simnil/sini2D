#pragma once
#ifndef SINI_SIMPLE_RENDERER_H
#define SINI_SIMPLE_RENDERER_H

#include <vector>

#include "sini/gl/glutil.hpp"
#include "sini/gl/GLContext.hpp"
#include "sini/gl/Camera.hpp"
#include "sini/sdl/Window.hpp"

namespace sini { namespace gl {
	
	class SimpleRenderer {
		public:
			// Public members
			// -----------------------------------------------------------------
			Camera camera;

			// Constructors
			// -----------------------------------------------------------------
			SimpleRenderer(const Window& window) noexcept;
			SimpleRenderer(const Window& window, Camera camera) noexcept;
			SimpleRenderer() noexcept = delete;
			~SimpleRenderer() noexcept = default;

			// Functions
			// -----------------------------------------------------------------
			// Clear the current canvas with the specified color (default is
			// black)
			void clear(vec4 clear_color = vec4(0.0f, 0.0f, 0.0f, 1.0f)) noexcept;

			// UNFINISHED
			void drawPolygon(std::vector<vec2> vertices, vec3 color, float alpha) noexcept;
			// TODO
			// void drawPolygon(std::vector<vec2> vertices, float width, vec3 color, float alpha) noexcept;
			// TODO
			// void fillPolygon(std::vector<vec2> vertices, vec3 color, float alpha) noexcept;

			// TODO
			// void drawRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha) noexcept;
			// TODO
			// void drawRectangle(vec2 bottom_left, vec2 upper_right, float width, vec3 color, float alpha) noexcept;
			// TODO
			// void fillRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha) noexcept;

			// TODO
			// void drawCircle(vec2 position, float radius, vec3 color, float alpha) noexcept;
			// TODO
			// void drawCircle(vec2 position, float radius, vec3 color, float alpha) noexcept;
			// TODO
			// void fillCircle(vec2 position, float radius, vec3 color, float alpha) noexcept;

			// Update the screen to make drawings visible (swap back and front
			// buffer). Wraps the SDL function SDL_GL_SwapWindow().
			void updateScreen() noexcept;

		private:
			// Private members
			// -----------------------------------------------------------------
			const Window* const window;
			const GLContext context;
			GLuint shader_program;
	};

}} // namespace gl, namespace sini
#endif
