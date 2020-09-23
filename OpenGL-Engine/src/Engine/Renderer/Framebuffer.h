#pragma once

#include "Engine/Core/Core.h"

namespace Engine {

	struct FramebufferSpecification {
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;
		bool SwapChainTarget = false; // ie. glBindFramebuffer(0)
		/*bool UsingDepthAttachment;
		bool UsingColourAttachment;
		bool UsingStencilAttachment;*/
		// TODO
	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual const uint32_t GetColourAttachment() const = 0;
		virtual const uint32_t GetDepthStencilAttachment() const = 0;

	private:


	};
}

