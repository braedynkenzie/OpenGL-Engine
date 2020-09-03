#include "EnginePCH.h"
#include "Buffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform\OpenGL\OpenGLBuffer.h"

namespace Engine {

	Ref<VertexBuffer> VertexBuffer::Create(unsigned __int32 size)
	{
		// Return a VertexBuffer implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a VertexBuffer.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a VertexBuffer.");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned __int32 size)
	{
		// Return a VertexBuffer implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a VertexBuffer.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a VertexBuffer.");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(unsigned __int32* indices, unsigned __int32 count)
	{
		// Return an IndexBuffer implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating an IndexBuffer.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create an IndexBuffer.");
		return nullptr;
	}

}
