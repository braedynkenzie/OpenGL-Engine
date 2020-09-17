#include "TestSpritesheetLayer.h"

#include "imgui/imgui.h"

TestSpritesheetLayer::TestSpritesheetLayer()
	: Layer("2D Spritesheet test"),
	m_CameraController(1280.0f / 720.0f, false),
	m_ModTime(0.0f)
{
}

void TestSpritesheetLayer::OnAttach()
{
	//m_TextureAtlas = Engine::Texture2D::Create("assets/textures/foliage_atlas.png");
	m_TextureAtlas = Engine::Texture2D::Create("assets/textures/rpg_map_atlas_2.png");

	//Engine::Ref<Engine::SubTexture2D>
	m_TextureGrassMid	= Engine::SubTexture2D::Create({ 3, 14 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassLeft	= Engine::SubTexture2D::Create({ 2, 14 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassRight	= Engine::SubTexture2D::Create({ 4, 14 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassTop	= Engine::SubTexture2D::Create({ 3, 15 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassBottom = Engine::SubTexture2D::Create({ 3, 13 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassLeftBottomCorner	= Engine::SubTexture2D::Create({ 2, 13 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassRightBottomCorner = Engine::SubTexture2D::Create({ 4, 13 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassRightTopCorner	= Engine::SubTexture2D::Create({ 4, 15 }, { 17, 17 }, m_TextureAtlas, 1);
	m_TextureGrassLeftTopCorner		= Engine::SubTexture2D::Create({ 2, 15 }, { 17, 17 }, m_TextureAtlas, 1);
}

void TestSpritesheetLayer::OnDetach()
{
}

void TestSpritesheetLayer::OnUpdate(Engine::Timestep deltaTime)
{
	ENGINE_PROFILE_FUNCTION();

	// ---------------------------------------------------------------
	// Update section ------------------------------------------------
	// ---------------------------------------------------------------

	// Process any camera movement or zoom changes
	m_CameraController.OnUpdate(deltaTime);

	// Reset Renderer2D statistics at the start of every frame
	Engine::Renderer2D::ResetStats();

	m_ModTime += deltaTime.GetSeconds();
	m_ModTime = fmod(m_ModTime, 1.0f);

	// ---------------------------------------------------------------
	// Render section ------------------------------------------------
	// ---------------------------------------------------------------
	{
		ENGINE_PROFILE_SCOPE("Render preparation");
		Engine::RenderCommand::SetClearColour({ 0.1f, 0.2f, 0.2f, 1.0f });
		Engine::RenderCommand::Clear();
	}

	{
		ENGINE_PROFILE_SCOPE("Rendering / draw calls");

		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		/*for(int x = 0; x < 57; x++)
			for(int y = 0; y < 31; y++)
				Engine::Renderer2D::DrawSpritesheetQuad({ x, y }, { 1.0f, 1.0f }, x, y, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);*/
		
		DrawGrassPatch(-5, -5, 14, 11);
		DrawTallGrassPatch(-0.5, -3.5, 3, 3);
		DrawTallGrassPatch(-1.75, -4.5, 4, 1);
		DrawTallGrassPatch(-4.0, -2.0, 2, 2);
		DrawTallGrassPatch(-2.75, -3.0, 2, 1);
		DrawTallGrassPatch(-1.9, -1.0, 2, 1);
		DrawTallGrassPatch(-2.25, -0.5, 1, 1);

		DrawTreeGreenRound(-4, -4);
		DrawTreeGreenRound(0.4, 1);
		DrawTreeGreenRound(-1.5f, -2);
		DrawTreeGreenRound(4.75f, 1.25f);
		DrawTreeGreenRound(5, 5);

		DrawTreeGreenPointy(-3, -4.5f);
		DrawTreeGreenPointy(-5, -2.5f);
		DrawTreeGreenPointy(3, 4.5f);
		DrawTreeGreenPointy(-3.2f, 0.2f);

		DrawTreeOrangeRound(3, -4.5f);
		DrawTreeOrangeRound(6.2f, -1.7f);
		DrawTreeOrangeRound(6.45f, -4.6);

		DrawTreeOrangePointy(5, -3.5f);
		DrawTreeOrangePointy(4, -2);
		DrawTreeOrangePointy(7.5f, -4);
		DrawTreeOrangePointy(7.0f, 1.4f);
		DrawTreeOrangePointy(5.5f, -1.0f);

		DrawTreeDead(-5.1f, 3.7f, 0);
		DrawTreeDead(-4.0f, 5.0f, 1);
		DrawTreeDead(-2.5f, 3.5f, 1);
		DrawTreeDead(-3.9f, 1.9f, 2);
		DrawTreeDead(-3.4f, 2.8f, 3);

		DrawCampfire(0, 0, fmod(m_ModTime, 0.33f) > 0.22f ? 2 : fmod(m_ModTime, 0.33f) > 0.11f ? 1 : 0);

		DrawWoodPile(-1.8f, 1.8f);
		DrawStumpAxe(-2.5f, 2.2f);

		DrawCobblePathVertical(2, 1);
		DrawCobblePathVertical(2, 2);
		DrawCobblePathVertical(2, 3);
		DrawCobblePathUpperRight(2, 4);
		DrawCobblePathHorizontal(1, 4);
		DrawCobblePathHorizontal(0, 4);
		DrawCobblePathBottomLeft(-1, 4);
		DrawCobblePathVertical(-1, 5);
		DrawCobblePathBottomLeft(2, 0);
		DrawCobblePathHorizontal(3, 0);
		DrawCobblePathHorizontal(4, 0);
		DrawCobblePathHorizontal(5, 0);
		DrawCobblePathHorizontal(6, 0);
		DrawCobblePathHorizontal(7, 0);
		DrawCobblePathHorizontal(8, 0);
		
		Engine::Renderer2D::EndScene();
	}
}

void TestSpritesheetLayer::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();
	
	ImGui::Begin("Info");

	Engine::Renderer2D::Statistics renderStats = Engine::Renderer2D::GetStats();
	ImGui::Text("Number of draw calls per frame: %i", renderStats.DrawCalls);
	ImGui::Text("Number of quads drawn per frame: %i", renderStats.QuadCount);

	ImGui::End();
}

void TestSpritesheetLayer::OnEvent(Engine::Event& event)
{
	ENGINE_PROFILE_FUNCTION();

	m_CameraController.OnEvent(event);
}

void TestSpritesheetLayer::DrawTreeGreenRound(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.5f }, { 1.0f, 1.0f }, 15, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos + 1.0f, 0.5f }, { 1.0f, 1.0f }, 15, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawTreeOrangeRound(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 14, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos + 1.0f, 0.5f }, { 1.0f, 1.0f }, 14, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawTreeGreenPointy(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.05f }, { 1.0f, 1.0f }, 18, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos + 1.0f, 0.5f }, { 1.0f, 1.0f }, 18, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawTreeOrangePointy(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 17, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos + 1.0f, 0.5f }, { 1.0f, 1.0f }, 17, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawTreeDead(float xPos, float yPos, uint8_t variant)
{
	switch (variant)
	{
		case 0:
			// Small dead tree
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, glm::vec2(1.0f) / 1.5f, 27, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
		case 1:
			// Tall dead tree
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, glm::vec2(1.0f) / 1.5f, 27, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos + 1.0f / 1.5f, 0.1f }, glm::vec2(1.0f) / 1.5f, 27, 21, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
		case 2:
			// Tree stump 1
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, glm::vec2(1.0f) / 1.5f, 53, 10, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
		case 3:
			// Tree stump 2
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, glm::vec2(1.0f) / 1.5f, 53, 11, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
	}
}

void TestSpritesheetLayer::DrawGrassPatch(float xPos, float yPos, float width, float height)
{
	// Middle grass
	float xMax = xPos + width - 1;
	float yMax = yPos + height - 1;
	for (float x = xPos + 1; x < xMax; x++)
		for (float y = yPos + 1; y < yMax; y++)
			Engine::Renderer2D::DrawSpritesheetQuad({ x, y }, { 1.0f, 1.0f }, m_TextureGrassMid);
	// Bottom grass
	for (float x = xPos + 1; x < xMax; x++)
		Engine::Renderer2D::DrawSpritesheetQuad({ x, yPos }, { 1.0f, 1.0f }, m_TextureGrassBottom);
	// Top grass
	for (float x = xPos + 1; x < xMax; x++)
		Engine::Renderer2D::DrawSpritesheetQuad({ x, yMax }, { 1.0f, 1.0f }, m_TextureGrassTop);
	// Left grass
	for (float y = yPos + 1; y < yMax; y++)
		Engine::Renderer2D::DrawSpritesheetQuad({ xPos, y }, { 1.0f, 1.0f }, m_TextureGrassLeft);
	// Right grass
	for (float y = yPos + 1; y < yMax; y++)
		Engine::Renderer2D::DrawSpritesheetQuad({ xMax, y }, { 1.0f, 1.0f }, m_TextureGrassRight);
	// Four corners of the grass
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos }, { 1.0f, 1.0f }, m_TextureGrassLeftBottomCorner);
	Engine::Renderer2D::DrawSpritesheetQuad({ xMax, yPos }, { 1.0f, 1.0f }, m_TextureGrassRightBottomCorner);
	Engine::Renderer2D::DrawSpritesheetQuad({ xMax, yMax }, { 1.0f, 1.0f }, m_TextureGrassRightTopCorner);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yMax }, { 1.0f, 1.0f }, m_TextureGrassLeftTopCorner);
}

void TestSpritesheetLayer::DrawTallGrassPatch(float xPos, float yPos, uint32_t width, uint32_t height)
{
	// Middle grass
	float xMax = xPos + width;
	float yMax = yPos + height;
	for (float x = xPos; x < xMax; x += 1.0f)
		for (float y = yPos; y < yMax; y += 1.0f)
		{
			Engine::Renderer2D::DrawSpritesheetQuad({ x, y, 0.2f }, glm::vec2(1.0f, 1.0f), 22, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			Engine::Renderer2D::DrawSpritesheetQuad({ x + 0.5f, y, 0.2f }, glm::vec2(1.0f, 1.0f), 22, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			Engine::Renderer2D::DrawSpritesheetQuad({ x + 0.75f, y + 0.5f, 0.2f }, glm::vec2(1.0f, 1.0f), 22, 19, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			Engine::Renderer2D::DrawSpritesheetQuad({ x + 0.25, y + 0.5f, 0.2f }, glm::vec2(1.0f, 1.0f), 22, 20, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
		}
}

void TestSpritesheetLayer::DrawCampfire(float xPos, float yPos, uint8_t frame)
{
	switch (frame)
	{
		case 0:
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 13, 22, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
		case 1:
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 14, 22, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
		case 2:
			Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 15, 22, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
			break;
	}
}

void TestSpritesheetLayer::DrawWoodPile(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, glm::vec2(1.0f) / 1.5f, 53, 8, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawStumpAxe(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, glm::vec2(1.0f) / 1.5f, 53, 9, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawMossyRock(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 54, 8, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawRock(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 54, 9, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawCobblePath(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos, 0.1f }, { 1.0f, 1.0f }, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawCobblePathHorizontal(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 13, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f),					yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 15, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 13, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos, yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 15, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 13, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f),					yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 15, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawCobblePathVertical(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 7, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 9, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 7, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 9, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 7, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 9, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawCobblePathUpperRight(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 9, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 9, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 15, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 15, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 9, 15, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}

void TestSpritesheetLayer::DrawCobblePathBottomLeft(float xPos, float yPos)
{
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 7, 13, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 13, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos - (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 13, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 7, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos, 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos - (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 7, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos,					yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
	Engine::Renderer2D::DrawSpritesheetQuad({ xPos + (1.0f / 3.0f), yPos + (1.0f / 3.0f), 0.1f }, glm::vec2(1.0f) / 3.0f, 8, 14, m_TextureAtlas, 968.0f, 526.0f, 17.0f, 17.0f, 1);
}


