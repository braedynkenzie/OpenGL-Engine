#pragma once

#include "Engine/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO: Add this in once we aren't including this file in sandbox
//#include <glad/glad.h>
// TODO: Remove this once we do the above
typedef unsigned int GLenum;

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vsSource, const std::string& fsSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformMat4(const std::string& name, const glm::mat4x4& matrix);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec4);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec3);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec2);
		void UploadUniformFloat1(const std::string& name, float val);
		void UploadUniformInt(const std::string& name, int val);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> ProcessShaderSource(const std::string& shaderSource);
		void Compile(const std::unordered_map<GLenum, std::string>& shadersMap);

	private:
		unsigned __int32 m_RendererID;
	};
}

