#pragma once

#include "Engine/Renderer/Framebuffer.h"

#include <glad\glad.h>

namespace Engine {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Reset();
		void Resize(uint32_t width, uint32_t height);

		void Bind() const override;
		void Unbind() const override;

		const FramebufferSpecification& GetSpecification() const override	{ return m_Specification; }
		const uint32_t GetColourAttachment() const override					{ return m_ColourBufferAttachment; }
		const uint32_t GetDepthStencilAttachment() const override			{ return m_DepthStencilBufferAttachment; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColourBufferAttachment = 0;
		uint32_t m_DepthStencilBufferAttachment = 0;
		FramebufferSpecification m_Specification;

	};
}

