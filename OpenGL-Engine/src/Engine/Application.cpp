#include "EnginePCH.h"
#include "Engine/Application.h"
#include "Engine/Log.h"
#include "Engine/Input.h"

#include <glad/glad.h>

namespace Engine {

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Assert that no previous instance of Application has been created
		ENGINE_CORE_ASSERT(!s_Instance, "Application (singleton) already exists!");
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {

			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			// Testing input polling
			//auto[x,y] = Input::GetMousePosition();
			//ENGINE_CORE_TRACE("{0}, {1}", x, y);
			
			m_Window->OnUpdate();
		}
	}

	// OnEvent is bound as the window event callback in the application constructor (above)
	void Application::OnEvent(Event& event)
	{
		// Log the event
		ENGINE_CORE_TRACE("{0}", event);

		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin(); )
		{
			(*--iterator)->OnEvent(event);
			if (event.Handled)
				break;
		}
		
		// Handle events using EventDispatcher
		EventDispatcher eventDispatcher = EventDispatcher(event);
		// Bind WindowCloseEvents to OnWindowCloseEvent method
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent)); 
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& WCEvent)
	{
		m_Running = false;
		return true;
	}
}