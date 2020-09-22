#pragma once

#include "Engine/Core/Core.h"

namespace Engine {

	struct FramebufferSpecification {
		uint32_t Width;
		uint32_t Height;
		bool UsingDepthAttachment;
		bool UsingColourAttachment;
		bool UsingStencilAttachment;
		// TODO
	};

	class Framebuffer
	{
	public:
		Ref<Framebuffer> Create(const FramebufferSpecification& spec);
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

	private:


	};
}

