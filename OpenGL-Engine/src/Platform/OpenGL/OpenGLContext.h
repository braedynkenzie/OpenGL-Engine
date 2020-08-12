#pragma once

#include "Engine\Renderer\RenderingContext.h"

struct GLFWwindow;

namespace Engine {
	
	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() override;
		void SwapBuffers() override;


	private:
		GLFWwindow* m_WindowHandle;

	};
}