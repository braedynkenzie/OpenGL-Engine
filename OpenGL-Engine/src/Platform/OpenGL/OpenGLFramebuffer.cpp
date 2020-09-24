#include "EnginePCH.h"
#include "OpenGLFramebuffer.h"

namespace Engine {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		Reset();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		// Clean up GPU memory
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColourBufferAttachment);
		glDeleteTextures(1, &m_DepthStencilBufferAttachment);
	}

	void OpenGLFramebuffer::Reset()
	{
		// If the framebuffer already has a RendererID, clean up old GPU memory first
		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColourBufferAttachment);
			glDeleteTextures(1, &m_DepthStencilBufferAttachment);
		}

		// Create and bind OpenGL framebuffer
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// Create and attach a colour buffer to this framebuffer
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourBufferAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColourBufferAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourBufferAttachment, 0);

		// Create and attach a depth / stencil buffer to this framebuffer
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilBufferAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilBufferAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilBufferAttachment, 0);

		// Confirm framebuffer is complete
		ENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Incomplete framebuffer!");

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Reset();
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
