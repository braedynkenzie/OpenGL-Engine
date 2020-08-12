
#include "Engine.h"
#include "imgui/imgui.h"

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

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
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
	}

	~Sandbox() {}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
