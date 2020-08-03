#include "EnginePCH.h"
#include "Engine/Application.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

namespace Engine {

	Engine::Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		//m_Window->SetEventCallback();
	}

	Engine::Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{

	}
}