#include "EnginePCH.h"
#include "Buffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform\OpenGL\OpenGLBuffer.h"

namespace Engine {

	VertexBuffer* VertexBuffer::Create(float* vertices, unsigned __int32 size)
	{
		// Return a VertexBuffer implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RenderingAPI::None:
			ENGINE_CORE_ASSERT(false, "RenderingAPI::None selected when creating a VertexBuffer.");
			return nullptr;
		case RenderingAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RenderingAPI enum selected when trying to create a VertexBuffer.");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(unsigned __int32* indices, unsigned __int32 count)
	{
		// Return an IndexBuffer implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RenderingAPI::None:
			ENGINE_CORE_ASSERT(false, "RenderingAPI::None selected when creating an IndexBuffer.");
			return nullptr;
		case RenderingAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RenderingAPI enum selected when trying to create an IndexBuffer.");
		return nullptr;
	}

}
