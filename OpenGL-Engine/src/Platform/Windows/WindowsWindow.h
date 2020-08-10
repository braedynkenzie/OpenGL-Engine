#pragma once

#include "Engine/Window.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		// Constructor/destrcutor
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		unsigned int GetWidth() const override { return m_WindowData.Width; }
		unsigned int GetHeight() const override { return m_WindowData.Height; }

		inline void* GetNativeWindow() const override { return m_Window; }

		// Window attribs
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }

	private:
		// Private functions
		virtual void Shutdown();
		virtual void Init(const WindowProperties& properties);

		// Member variables
		GLFWwindow* m_Window;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_WindowData;

	};
}

