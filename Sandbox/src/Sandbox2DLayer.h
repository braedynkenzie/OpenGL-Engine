#pragma once

#include "Engine.h"

class Sandbox2DLayer : public Engine::Layer
{
public:
	Sandbox2DLayer();
	~Sandbox2DLayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Engine::Timestep deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& event) override;

private:
	Engine::OrthographicCameraController m_CameraController;
	glm::vec4 m_QuadColour = { 0.2f, 0.2f, 0.8f, 1.0f };

};

