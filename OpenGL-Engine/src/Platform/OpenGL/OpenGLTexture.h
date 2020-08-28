#pragma once

#include "Engine\Renderer\Texture.h"

#include <glad\glad.h>

namespace Engine {
	
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		void Bind(uint32_t slot) const override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void SetData(void* data, uint32_t size) override;

	private:
		std::string m_Filepath;
		unsigned __int32 m_RendererID;
		unsigned __int32 m_Width;
		unsigned __int32 m_Height;
		GLenum m_InternalFormat, m_DataFormat;
	};
}


