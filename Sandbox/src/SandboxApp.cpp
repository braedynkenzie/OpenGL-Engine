
//namespace Engine {
//	__declspec(dllimport) void Print(const char* msg);
//}

#include "Engine.h"

class Sandbox : public Engine::Application 
{
public:
	Sandbox(){}
	~Sandbox(){}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
