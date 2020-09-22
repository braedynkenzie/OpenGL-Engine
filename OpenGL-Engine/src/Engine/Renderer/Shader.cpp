#include "EnginePCH.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Engine {

	// ------------------------
	// Shader -----------------
	// ------------------------

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		ENGINE_PROFILE_FUNCTION();

		// Return a Shader implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a Shader.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a Shader.");
		return nullptr;
	}
	
	Ref<Shader> Shader::Create(const std::string name, const std::string& vsSource, const std::string& fsSource)
	{
		ENGINE_PROFILE_FUNCTION();

		// Return a Shader implementation 
		// Depends on the currently used Rendering API
		switch (Renderer::GetRenderingAPI())
		{
		case RendererAPI::API::None:
			ENGINE_CORE_ASSERT(false, "RendererAPI::API::None selected when creating a Shader.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vsSource, fsSource);
		}
		ENGINE_CORE_ASSERT(false, "Invalid RendererAPI::API enum selected when trying to create a Shader.");
		return nullptr;
	}


	// ---------------------------------------------------------
	// ShaderLibrary -------------------------------------------
	// ---------------------------------------------------------

	void ShaderLibrary::Add(const std::string& shaderName, const Ref<Shader>& newShader)
	{
		ENGINE_PROFILE_FUNCTION();

		ENGINE_CORE_ASSERT(!Exists(shaderName), "Shader already exists in the ShaderLibrary!");
		m_Shaders[shaderName] = newShader;
	}
	
	void ShaderLibrary::Add(const Ref<Shader>& newShader)
	{
		ENGINE_PROFILE_FUNCTION();

		const std::string& shaderName = newShader->GetName();
		Add(shaderName, newShader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		ENGINE_PROFILE_FUNCTION();

		Ref<Shader> shader = Shader::Create(filepath);
		this->Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& shaderName, const std::string& filepath)
	{
		ENGINE_PROFILE_FUNCTION();

		Ref<Shader> shader = Shader::Create(filepath);
		this->Add(shaderName, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& shaderName)
	{
		ENGINE_PROFILE_FUNCTION();

		ENGINE_CORE_ASSERT(Exists(shaderName), "Shader does not exist in the ShaderLibrary!");
		return m_Shaders[shaderName];
	}

	const bool ShaderLibrary::Exists(std::string shaderName) const
	{
		ENGINE_PROFILE_FUNCTION();

		return (m_Shaders.find(shaderName) != m_Shaders.end());
	}
}