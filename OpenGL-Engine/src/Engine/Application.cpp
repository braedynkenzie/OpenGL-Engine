#include "Engine/Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

namespace Engine {

	Engine::Application::Application()
	{
	}

	Engine::Application::~Application()
	{
	}

	void Application::Run()
	{
		// Testing Event System and Logging
		WindowResizeEvent wre = WindowResizeEvent(16, 9);
		ENGINE_TRACE(wre); 
		ENGINE_WARN(wre);

		while (true) {
			// Keep window open
		}
	}
}