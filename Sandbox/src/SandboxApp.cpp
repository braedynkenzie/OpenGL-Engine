
#include "Engine.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer") {}

	void OnUpdate() override
	{
		// ENGINE_INFO("Example layer OnUpdate called!");

		// Testing input polling and OpenGL-Engine keycodes
		if (Engine::Input::IsKeyPressed(KEY_TAB))
			ENGINE_INFO("Tab key is currently pressed!");
	}

	void OnEvent(Engine::Event& event) override
	{
		// ENGINE_TRACE("Event passed to Example Layer: {0}", event);
	}
};

class Sandbox : public Engine::Application 
{
public:
	Sandbox()
	{
		this->PushLayer(new ExampleLayer());
		this->PushOverlay(new Engine::ImGuiLayer());
	}

	~Sandbox() {}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
