#pragma once

#include <string>

namespace Engine {

	class Shader
	{
	public:
		Shader(const std::string& vsSource, const std::string& fsSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned __int32 m_RendererID;

	};

}
