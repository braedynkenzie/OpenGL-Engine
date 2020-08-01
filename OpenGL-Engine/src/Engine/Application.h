#pragma once

#include "Engine/Core.h"
#include "Events/Event.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	// To be define in the engine client
	Application* CreateApplication();
}

