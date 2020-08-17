#include "EnginePCH.h"
#include "VertexArray.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform\OpenGL\OpenGLBuffer.h"
#include "Platform\OpenGL\OpenGLVertexArray.h"

namespace Engine {

	VertexArray* VertexArray::Create()
	{
		// Return a VertexArray implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a VertexArray.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a VertexArray.");
		return nullptr;
	}
}
