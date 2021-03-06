#include "EnginePCH.h"
#include "WindowsWindow.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

#include "Platform\OpenGL\OpenGLContext.h"

namespace Engine {

	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		ENGINE_CORE_ERROR("GLFW Error code: [{0}], {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& properties) 
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties) 
	{
		ENGINE_PROFILE_FUNCTION();

		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		ENGINE_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		ENGINE_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		ENGINE_PROFILE_FUNCTION();

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
		ENGINE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		ENGINE_PROFILE_FUNCTION();

		m_WindowData.Title = properties.Title;
		m_WindowData.Width = properties.Width;
		m_WindowData.Height = properties.Height;

		ENGINE_CORE_INFO("Creating a Windows platform window: {0} ({1}, {2})", m_WindowData.Title, m_WindowData.Width, m_WindowData.Height);

		if (!s_GLFWInitialized)
		{
			// TODO terminate on shutdown
			int success;
			{
				ENGINE_PROFILE_SCOPE("glfwInit() in WindowsWindow::Init()");
				success = glfwInit();
			}
			ENGINE_CORE_ASSERT(success, "Could not init GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		{
			ENGINE_PROFILE_SCOPE("glfwCreateWindow() in WindowsWindow::Init()");
			m_Window = glfwCreateWindow((int)m_WindowData.Width, (int)m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
		}

		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_WindowData);
		SetVSync(true);

		// Dispatch all GLFW callbacks to the OpenGL-Engine's event callbacks
		// -------------------------------------------------------------------
		// Window resize callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.Width = width;
			windowData.Height = height;
			// Create the OpenGL-Engine Event
			WindowResizeEvent windowResizeEvent = WindowResizeEvent(width, height);
			// Call the OnEvent function that is set in Application.cpp
			windowData.EventCallback(windowResizeEvent);
		});
		// Window close callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			// Create the OpenGL-Engine Event
			WindowCloseEvent windowCloseEvent = WindowCloseEvent();
			// Call the OnEvent function that is set in Application.cpp
			windowData.EventCallback(windowCloseEvent);
		});
		// Window key pressed callback
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			// Switch statement based on key action type (key pressed, released, held down, etc.)
			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressEvent keyPressEvent = KeyPressEvent(key, 0);
					windowData.EventCallback(keyPressEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent keyReleaseEvent = KeyReleaseEvent(key);
					windowData.EventCallback(keyReleaseEvent);
					break;
				}
				case GLFW_REPEAT:
				{
					// TODO: extract proper key repeat count in KeyPressEvent constructor below
					KeyPressEvent keyHeldEvent = KeyPressEvent(key, 1);
					windowData.EventCallback(keyHeldEvent);
					break;
				}
			}
		});
		// Window key typed callback
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent ktEvent = KeyTypedEvent(keyCode);
			windowData.EventCallback(ktEvent);
		});
		// Window mouse button callback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			// Switch statement based on mouse button action type (pressed, released, held down, etc.)
			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressEvent mouseButtonPressEvent = MouseButtonPressEvent(button);
					windowData.EventCallback(mouseButtonPressEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleaseEvent mouseButtonReleaseEvent = MouseButtonReleaseEvent(button);
					windowData.EventCallback(mouseButtonReleaseEvent);
					break;
				}
			}
		});
		// Window mouse scroll callback
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrollEvent mouseScrollEvent = MouseScrollEvent((float)xOffset, (float)yOffset);
			windowData.EventCallback(mouseScrollEvent);
		});
		// Window mouse cursor move callback
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			// Get the WindowData struct we previously bound to the GLFW window
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMoveEvent mouseMoveEvent = MouseMoveEvent((float)xPos, (float)yPos);
			windowData.EventCallback(mouseMoveEvent);
		});
	}

}
