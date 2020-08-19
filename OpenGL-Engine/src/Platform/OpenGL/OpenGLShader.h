#pragma once

#include "Engine/Renderer/Shader.h"

#include "glm/glm.hpp"

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
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
		unsigned __int32 m_RendererID;
	};
}

