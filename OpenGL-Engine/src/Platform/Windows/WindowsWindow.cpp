#include "EnginePCH.h"
#include "WindowsWindow.h"

namespace Engine {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProperties& properties) 
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties) 
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_WindowData.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_WindowData.VSync;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		m_WindowData.Title = properties.Title;
		m_WindowData.Width = properties.Width;
		m_WindowData.Height = properties.Height;

		ENGINE_CORE_INFO("Creating a Windows platform window: {0} ({1}, {2})", m_WindowData.Title, m_WindowData.Width, m_WindowData.Height);

		if (!s_GLFWInitialized)
		{
			// TODO terminate on shutdown
			int success = glfwInit();
			ENGINE_CORE_ASSERT(success, "Could not init GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_WindowData.Width, (int)m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowData);
		SetVSync(true);
	}

}
