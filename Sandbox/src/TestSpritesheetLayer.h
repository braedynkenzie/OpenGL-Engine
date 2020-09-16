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

	// -------------------------------------------------------
	// Spritesheet functions ---------------------------------
	// -------------------------------------------------------
	void DrawGrassPatch(float xPos, float yPos, float width, float height);
	void DrawTallGrassPatch(float xPos, float yPos, uint32_t width, uint32_t height);
	
	void DrawTreeGreenRound(float xPos, float yPos);
	void DrawTreeOrangeRound(float xPos, float yPos);
	void DrawTreeGreenPointy(float xPos, float yPos);
	void DrawTreeOrangePointy(float xPos, float yPos);
	void DrawTreeDead(float xPos, float yPos, uint8_t variant);
	
	void DrawCampfire(float xPos, float yPos, uint8_t frame);

	void DrawWoodPile(float xPos, float yPos);
	void DrawStumpAxe(float xPos, float yPos);
	void DrawMossyRock(float xPos, float yPos);
	void DrawRock(float xPos, float yPos);

	void DrawCobblePath(float xPos, float yPos);
	void DrawCobblePathHorizontal(float xPos, float yPos);
	void DrawCobblePathVertical(float xPos, float yPos);
	void DrawCobblePathUpperRight(float xPos, float yPos);
	void DrawCobblePathBottomLeft(float xPos, float yPos);

private:
	Engine::OrthographicCameraController m_CameraController;
	Engine::Ref<Engine::Texture2D> m_TextureAtlas;
	float m_ModTime;

};

