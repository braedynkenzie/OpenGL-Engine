#include "EnginePCH.h"
#include "RendererAPI.h"

namespace Engine {

	// Initialize static variable
	// Hardcoding the rendering API as OpenGL for now
	RendererAPI::API RendererAPI::s_RenderingAPI = RendererAPI::API::OpenGL;

}