#include "Sandbox2DLayer.h"

#include "imgui/imgui.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\ext\matrix_transform.hpp>

// TEMPORARY
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("2D Sandbox"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2DLayer::OnAttach()
{
}

void Sandbox2DLayer::OnDetach()
{
}

void Sandbox2DLayer::OnUpdate(Engine::Timestep deltaTime)
{
	// ---------------------------------------------------------------
	// Update section ------------------------------------------------
	// ---------------------------------------------------------------

	// Process any camera movement or zoom changes
	m_CameraController.OnUpdate(deltaTime);

	// ---------------------------------------------------------------
	// Render section ------------------------------------------------
	// ---------------------------------------------------------------
	Engine::RenderCommand::SetClearColour({ 0.1f, 0.2f, 0.2f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	int numX = 10;
	int numY = 10;
	for (int x = 0; x < numX; x++)
	{
		for (int y = 0; y < numY; y++)
		{
			Engine::Renderer2D::DrawQuad({ x, y }, { 0.99f, 0.99f }, m_QuadColour + glm::vec4((float)x / (float)numX, (float)y / (float)numY, 0.0f, 0.0f));
		}
	}

	Engine::Renderer2D::EndScene();
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Quad Colour", glm::value_ptr(m_QuadColour));
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
