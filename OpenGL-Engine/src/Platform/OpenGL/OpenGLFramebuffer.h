#pragma once

#include "Engine/Renderer/Framebuffer.h"

namespace Engine {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);

		void Bind() const override;

		const FramebufferSpecification& GetSpecification() const override;

	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_FramebufferSpecification;

	};
}

