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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}