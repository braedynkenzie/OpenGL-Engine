#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	// Initialize Engine systems
	Engine::Log::Init();

	printf("OpenGL-Engine Entry Point start\n");
	Engine::Application* engineApplication = Engine::CreateApplication();
	engineApplication->Run();
	delete engineApplication;
}

#endif
