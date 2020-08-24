#pragma once

#include <string>
#include <unordered_map>

namespace Engine {

	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string name, const std::string& vsSource, const std::string& fsSource);
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& shaderName, const Ref<Shader>& newShader);
		void Add(const Ref<Shader>& newShader);
		Ref<Shader> Load(const std::string& filepath); // will be named the shader's filename by default
		Ref<Shader> Load(const std::string& shaderName, const std::string& filepath);
		//void Remove(std::string shaderName);
		Ref<Shader> Get(const std::string& shaderName);
		const bool Exists(std::string shaderName) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};

}
