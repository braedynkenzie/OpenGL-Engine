#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"

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

