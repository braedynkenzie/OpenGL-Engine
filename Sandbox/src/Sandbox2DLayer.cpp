#include "Sandbox2DLayer.h"

#include "imgui/imgui.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\ext\matrix_transform.hpp>

// TEMPORARY
//#include <chrono>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("2D Sandbox"), 
	m_CameraController(1280.0f / 720.0f, true),
	m_NumRows(10),
	m_NumColumns(10),
	m_BatchSize(100)
{
}

void Sandbox2DLayer::OnAttach()
{
	ENGINE_PROFILE_FUNCTION();

	// Debugging
	m_TestTexture = Engine::Texture2D::Create("assets/textures/background_image.png");
}

void Sandbox2DLayer::OnDetach()
{
	ENGINE_PROFILE_FUNCTION();
}

void Sandbox2DLayer::OnUpdate(Engine::Timestep deltaTime)
{
	ENGINE_PROFILE_FUNCTION();

	// ---------------------------------------------------------------
	// Update section ------------------------------------------------
	// ---------------------------------------------------------------
	
	// Process any camera movement or zoom changes
	m_CameraController.OnUpdate(deltaTime);

	// Reset Renderer2D statistics at the start of every frame
	Engine::Renderer2D::ResetStats();

	// TODO requires a fix to resize s_Data.QuadVertexBufferBase/Ptr size
	//Engine::Renderer2D::SetBatchCount(m_BatchSize);

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

		for (int x = 0; x < m_NumColumns; x++)
		{
			for (int y = 0; y < m_NumRows; y++)
			{
				Engine::Renderer2D::DrawQuad({ x, y }, { 0.75f, 0.75f }, m_QuadColour + glm::vec4((float)x / (float)m_NumRows, (float)y / (float)m_NumColumns, 0.0f, 0.0f));
			}
		}

		// debugging
		//Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		//Engine::Renderer2D::DrawTexturedQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_TestTexture);
		//Engine::Renderer2D::DrawTexturedQuad({ -0.8f, -0.8f, 0.1f }, { 0.5f, 0.5f }, m_TestTexture);

		Engine::Renderer2D::EndScene();
	}
}

void Sandbox2DLayer::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Base Colour", glm::value_ptr(m_QuadColour));
	ImGui::DragInt("Num rows", (int*)&m_NumRows, 0.1f, 1, 100);
	ImGui::DragInt("Num columns", (int*)&m_NumColumns, 0.1f, 1, 100);
	//ImGui::DragInt("Batch size", (int*)&m_BatchSize, 0.5f, 1, 10000);

	Engine::Renderer2D::Statistics renderStats = Engine::Renderer2D::GetStats();
	ImGui::Text("Number of draw calls per frame: %i", renderStats.DrawCalls);
	ImGui::Text("Number of quads drawn per frame: %i", renderStats.QuadCount);

	// Display performance profiling results
	/*for (auto result : m_ProfilingResults)
	{
		char label[50];
		strcpy(label, "%.4f ms, ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Duration);
	}
	m_ProfilingResults.clear();*/

	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
