#include "Engine.h"

#include "imgui/imgui.h"

#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\ext\matrix_transform.hpp>

// TEMPORARY
#include "TestGameLayer.h"
#include "Sandbox2DLayer.h"
#include "ExampleLayer.h"

// ---- Engine Entry Point --------
#include "Engine/Core/EntryPoint.h"
// --------------------------------

class Sandbox : public Engine::Application 
{
public:
	Sandbox()
	{
		//this->PushLayer(new TestGameLayer());
		this->PushLayer(new Sandbox2DLayer());
		//this->PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
