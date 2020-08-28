#include "EnginePCH.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Engine {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		m_DataFormat = GL_RGBA;
		m_InternalFormat = GL_RGBA8;

		// Create the texture ID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// Create the storage space for the texture on the GPU
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		// Set the filtering properties for the texture
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Set the texture wrapping mode
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_Width(0), m_Height(0), m_Filepath(filepath)
	{
		// First use stb_image header-only library to load the texture from a filepath
		int width, height;
		int channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* imageData = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 0);
		ENGINE_CORE_ASSERT(imageData, "Failed to load texture!");
		m_Width = width;
		m_Height = height;

		GLenum dataFormat = 0, internalFormat = 0;
		if (channels == 4)
		{
			dataFormat		= GL_RGBA;
			internalFormat	= GL_RGBA8;
		}
		else if(channels == 3)
		{
			dataFormat		= GL_RGB;
			internalFormat	= GL_RGB8;
		}
		ENGINE_CORE_ASSERT(dataFormat & internalFormat, "Number of texture channels not supported!");
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		// Create the texture ID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// Create the storage space for the texture on the GPU
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		// Set the filtering properties for the texture
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Set the texture wrapping mode
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Now pass the texture data to the GPU memory
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, imageData);

		// Finally, release the data from CPU memory
		stbi_image_free(imageData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	
	void Engine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		// Assert that the size is correct
		uint32_t bytesPerPixel = m_DataFormat = GL_RGBA ? 4 : 3;
		ENGINE_CORE_ASSERT(size == (m_Width * m_Height * bytesPerPixel), "Data must have same size as entire texture!");
		// Pass texture data to the GPU memory
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}
