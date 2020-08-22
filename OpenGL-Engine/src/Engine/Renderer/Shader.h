#pragma once

#include <string>

namespace Engine {

	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& vsSource, const std::string& fsSource);
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

}
