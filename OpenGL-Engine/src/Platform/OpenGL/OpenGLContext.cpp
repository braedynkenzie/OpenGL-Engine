#include "EnginePCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_WindowHandle(window)
	{
		ENGINE_CORE_ASSERT(m_WindowHandle, "Window handle for OpenGL rendering context is invalid!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		// Initialize Glad (links OpenGL functions from graphics drivers)
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_CORE_ASSERT(status, "Initializing Glad");

		ENGINE_CORE_INFO("OpenGL graphics drivers: {0}, {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		ENGINE_CORE_INFO("OpenGL version: {0}", glGetString(GL_VERSION));

		#ifdef ENGINE_ENABLE_ASSERTS
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			ENGINE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Engine requires at least OpenGL version 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}
