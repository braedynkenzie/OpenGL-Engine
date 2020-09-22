#include "EnginePCH.h"
#include "Framebuffer.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Engine {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		ENGINE_PROFILE_FUNCTION();

		// Return a Framebuffer implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a Framebuffer.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a Framebuffer.");
		return nullptr;
	}
}
