#pragma once

#include "Engine.h"

class TestSpritesheetLayer : public Engine::Layer
{
public:
	TestSpritesheetLayer();
	~TestSpritesheetLayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Engine::Timestep deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& event) override;

private:
	Engine::OrthographicCameraController m_CameraController;
	Engine::Ref<Engine::Texture2D> m_TextureAtlas;

};

