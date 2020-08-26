#include "Sandbox2DLayer.h"

#include "imgui/imgui.h"

#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\ext\matrix_transform.hpp>

// TEMPORARY
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("2D Sandbox"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2DLayer::OnAttach()
{
	m_QuadVA = Engine::VertexArray::Create();

	float quadVertices[] = {
			0.5f,  0.5f,  0.0f,
		   -0.5f,  0.5f,  0.0f,
			0.5f, -0.5f,  0.0f,
		   -0.5f, -0.5f,  0.0f
	};

	Engine::Ref<Engine::VertexBuffer> quadVB;
	quadVB.reset(Engine::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
	quadVB->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" }
	});
	m_QuadVA->AddVertexBuffer(quadVB);

	uint32_t quadIndices[6] = { 0, 1, 2,
								2, 1, 3 };
	Engine::Ref<Engine::IndexBuffer> quadIB;
	quadIB.reset(Engine::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
	m_QuadVA->SetIndexBuffer(quadIB);

	m_FlatColourShader = Engine::Shader::Create("assets/shaders/FlatColour.glsl");
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

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColourShader)->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Colour", m_QuadColour);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	Engine::Renderer::Submit(m_FlatColourShader, m_QuadVA, modelMatrix);

	Engine::Renderer::EndScene();
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
