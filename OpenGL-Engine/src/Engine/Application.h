#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Window.h"
#include "Engine/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine\Renderer\Buffer.h"
#include "Engine\Renderer\VertexArray.h"

// TEMPORARY
#include <Engine\Renderer\Shader.h>

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

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& GetInstance() { return *s_Instance; }

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& WCEvent);

	private:
		static Application* s_Instance;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		std::shared_ptr<VertexArray> m_VertexArray;

		// TEMPORARY
		std::shared_ptr<Shader> m_Shader;

	};

	// To be defined in the engine client
	Application* CreateApplication();
}

