#pragma once
#ifndef SINI_SIMPLE_RENDERER_H
#define SINI_SIMPLE_RENDERER_H

#include "sini/gl/glutil.hpp"
#include "sini/gl/GLContext.hpp"
#include "sini/gl/Camera.hpp"

namespace sini { namespace gl {
	
	class SimpleRenderer {
		public:
			// Public members
			// -----------------------------------------------------------------
			Camera camera = Camera({ 0.0f, 0.0f }, 16.0f/9.0f, 2.0f);

			// Constructors
			// -----------------------------------------------------------------
			SimpleRenderer(GLuint context) noexcept;
			SimpleRenderer(GLuint context, Camera camera) noexcept;
			~SimpleRenderer() noexcept = default;

			// Functions
			// -----------------------------------------------------------------
		private:
			// Private members
			// -----------------------------------------------------------------
			GLuint context_handle = 0;
		};

}} // namespace gl, namespace sini
#endif