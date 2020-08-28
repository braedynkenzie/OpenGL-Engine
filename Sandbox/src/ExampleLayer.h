#pragma once

#include "Engine.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer();
	~ExampleLayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Engine::Timestep deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& event) override;

	bool OnWindowResizeEvent(Engine::WindowResizeEvent wrEvent);

private:
	Engine::OrthographicCameraController m_CameraController;
	glm::vec3 m_TrianglePosition;
	float m_TriangleRotation;
	float m_ModelSpeed;
	float m_RotationSpeed;
	glm::vec4 m_PulseColour;
	Engine::Ref<Engine::VertexArray> m_QuadVertexArray;
	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::ShaderLibrary m_ShaderLibrary;

};
