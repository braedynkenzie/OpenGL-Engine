#include "EnginePCH.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Engine {

	Shader* Shader::Create(const std::string& vsSource, const std::string& fsSource)
	{
		// Return a Shader implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a Shader.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vsSource, fsSource);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a Shader.");
		return nullptr;
	}
}