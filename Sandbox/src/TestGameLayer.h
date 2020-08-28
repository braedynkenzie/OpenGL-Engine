#pragma once

#include "Engine.h"

struct TreeData {
	glm::vec3 Position;
	glm::vec2 Scale;
	Engine::Ref<Engine::Texture2D> Texture;
};

class TestGameLayer : public Engine::Layer
{
public:
	TestGameLayer();
	~TestGameLayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Engine::Timestep deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& event) override;

	bool OnKeyPressedEvent(Engine::KeyPressEvent kpEvent);
	
	void AddNewTree();

private:
	Engine::OrthographicCameraController m_CameraController;
	glm::vec4 m_ClearColour;
	glm::vec3 m_StartingRabbitPosition, m_RabbitPosition, m_RabbitVelocity;
	std::vector<glm::vec3> m_TreePositions;
	float m_RabbitJumpPower, m_RabbitGravity, m_RabbitAngle, m_RabbitAirSpeed, m_ObstacleSpeed, m_MaxFallSpeed;
	float m_ModTime;

	Engine::Ref<Engine::Texture2D> m_RabbitTexture, m_GrassTexture, m_GroundTexture, m_BackgroundTexture;
	std::array<Engine::Ref<Engine::Texture2D>, 8> m_TreeTextures;
	std::vector<TreeData> m_Trees;
};