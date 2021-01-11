#pragma once

#include "EnginePCH.h"
#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"

namespace Engine
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(std::string title = "Engine window",
						uint32_t width = 1600, // 1280,
						uint32_t height = 900) // 720)
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
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void* GetNativeWindow() const = 0;
		// Window attribs
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

	};
}
