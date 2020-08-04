#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Window.h"
#include "Engine/LayerStack.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool m_Running = true;
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;

		bool OnWindowCloseEvent(WindowCloseEvent& WCEvent);

	};

	// To be defined in the engine client
	Application* CreateApplication();
}

