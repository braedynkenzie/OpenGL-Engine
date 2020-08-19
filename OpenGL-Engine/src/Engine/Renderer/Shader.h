#pragma once

#include <string>

namespace Engine {

	class Shader
	{
	public:
		static Shader* Create(const std::string& vsSource, const std::string& fsSource);
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

}
