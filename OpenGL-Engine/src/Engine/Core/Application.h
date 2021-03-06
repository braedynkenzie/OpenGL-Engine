#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Core/Timestep.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application(const std::string& name = "Engine Application");
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& GetInstance() { return *s_Instance; }

		void Close();

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& wcEvent);
		bool OnWindowResizeEvent(WindowResizeEvent& wrEvent);

	private:
		static Application* s_Instance;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;

		float m_LastFrameTime;
		Timestep m_Timestep;
	};

	// To be defined in the engine client
	Application* CreateApplication();
}

