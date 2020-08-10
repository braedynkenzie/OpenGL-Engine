#pragma once

#include "EnginePCH.h"
#include "Engine/Core.h"
#include "Engine/Events/Event.h"

namespace Engine
{
	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(std::string title = "OpenGL-Engine",
						unsigned int width = 1280,
						unsigned int height = 720) 
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface for a desktop system based Window
	class ENGINE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		static Window* Create(const WindowProperties& properties = WindowProperties());

		// Pure virtual functions below
		// -----------------------------
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void* GetNativeWindow() const = 0;
		// Window attribs
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

	};
}
