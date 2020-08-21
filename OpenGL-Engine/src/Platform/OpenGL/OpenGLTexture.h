#pragma once

#include "Engine\Renderer\Texture.h"

namespace Engine {
	
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot) const override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

	private:
		std::string m_Filepath;
		unsigned __int32 m_RendererID;
		unsigned __int32 m_Width;
		unsigned __int32 m_Height;
	};
}


