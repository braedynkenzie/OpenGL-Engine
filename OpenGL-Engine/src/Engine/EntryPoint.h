#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	// Initialize Engine systems
	Engine::Log::Init();

	// Testing
	ENGINE_CORE_WARN("Core logger warn level log test");
	ENGINE_CRITICAL("Client logger critical level log test");

	printf("OpenGL-Engine Entry Point start\n");
	auto application = Engine::CreateApplication();
	application->Run();
	delete application;
}

#endif
