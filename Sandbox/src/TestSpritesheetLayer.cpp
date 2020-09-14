#include "TestSpritesheetLayer.h"

#include "imgui/imgui.h"

TestSpritesheetLayer::TestSpritesheetLayer()
	: Layer("2D Spritesheet test"),
	m_CameraController(1280.0f / 720.0f, false)
{
}

void TestSpritesheetLayer::OnAttach()
{
	//m_TextureAtlas = Engine::Texture2D::Create("assets/textures/foliage_atlas.png");
	m_TextureAtlas = Engine::Texture2D::Create("assets/textures/rpg_map_atlas.png");
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
		/*Engine::Renderer2D::DrawSpritesheetQuad({  0.0f, 0.0f }, { 1.0f, 1.0f }, 14, 12, m_TextureAtlas);
		Engine::Renderer2D::DrawSpritesheetQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, 16, 14, m_TextureAtlas);
		Engine::Renderer2D::DrawSpritesheetQuad({ -2.0f, 0.0f }, { 1.0f, 1.0f }, 13, 14, m_TextureAtlas);
		Engine::Renderer2D::DrawSpritesheetQuad({ -3.0f, 0.0f }, { 1.0f, 1.0f }, 13, 14, m_TextureAtlas);
		Engine::Renderer2D::DrawSpritesheetQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, 22, 22, m_TextureAtlas);*/
		for(int x = 0; x < 57; x++)
			for(int y = 0; y < 31; y++)
				Engine::Renderer2D::DrawSpritesheetQuad({ x, y }, { 1.0f, 1.0f }, x, y, m_TextureAtlas);
			
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
