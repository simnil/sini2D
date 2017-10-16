#include "sini/gl/SimpleRenderer.hpp"

namespace sini { namespace gl {

	// Constructors
	// -------------------------------------------------------------------------
	SimpleRenderer::SimpleRenderer(GLuint context) noexcept
		: context_handle(context)
	{
		glewInit();
	}

	SimpleRenderer::SimpleRenderer(GLuint context, Camera camera) noexcept
		: context_handle(context), camera(camera)
	{
		glewInit();
	}

	// Functions
	// -------------------------------------------------------------------------
}}