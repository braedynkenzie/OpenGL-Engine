#pragma once

#include "Engine/Renderer/Framebuffer.h"

#include <glad\glad.h>

namespace Engine {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Resize();
		void Bind() const override;
		void Unbind() const override;

		const FramebufferSpecification& GetSpecification() const override	{ return m_Specification; }
		const uint32_t GetColourAttachment() const override					{ return m_ColourBufferAttachment; }
		const uint32_t GetDepthStencilAttachment() const override			{ return m_DepthStencilBufferAttachment; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColourBufferAttachment;
		uint32_t m_DepthStencilBufferAttachment;
		FramebufferSpecification m_Specification;

	};
}

