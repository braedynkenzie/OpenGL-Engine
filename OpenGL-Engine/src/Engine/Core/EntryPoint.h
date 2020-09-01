#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	// Initialize Engine systems
	//
	Engine::Log::Init();

	// Create and run an application
	//
	ENGINE_PROFILE_BEGIN_SESSION("Application Startup", "EngineProfiling-Startup.json");
	Engine::Application* engineApplication = Engine::CreateApplication();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Application Runtime", "EngineProfiling-Runtime.json");
	engineApplication->Run();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Application Shutdown", "EngineProfiling-Shutdown.json");
	delete engineApplication;
	ENGINE_PROFILE_END_SESSION();
}

#endif
