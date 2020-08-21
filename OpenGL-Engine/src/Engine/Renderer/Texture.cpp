#include "EnginePCH.h"
#include "Texture.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine {

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		// Return a Texture2D implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a Texture2D.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(filepath);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a Texture2D.");
		return nullptr;
	}
}