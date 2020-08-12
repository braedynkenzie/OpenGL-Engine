#pragma once

namespace Engine {

	// Abstracted rendering API context, to be implemented as OpenGLContext, VulkanContext, Directx11/12Contexts, etc.
	class RenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	private:


	};

}

