#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Engine {

	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string name, const std::string& vsSource, const std::string& fsSource);
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt   (const std::string& name, const int   value) = 0;
		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) = 0;
		virtual void SetFloat (const std::string& name, const float value) = 0;
		virtual void SetMat3  (const std::string& name, const glm::mat3    matrix3) = 0;
		virtual void SetMat4  (const std::string& name, const glm::mat4    matrix4) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3    float3)  = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4    float4)  = 0;

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
