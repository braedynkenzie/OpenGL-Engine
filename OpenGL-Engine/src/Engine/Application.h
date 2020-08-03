#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Window.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};

	// To be defined in the engine client
	Application* CreateApplication();
}

