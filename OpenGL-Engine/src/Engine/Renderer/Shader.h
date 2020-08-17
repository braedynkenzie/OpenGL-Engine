#pragma once

#include <string>
#include <glm\glm.hpp>

namespace Engine {

	class Shader
	{
	public:
		Shader(const std::string& vsSource, const std::string& fsSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4x4& matrix);

	private:
		unsigned __int32 m_RendererID;

	};

}
