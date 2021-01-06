#include "ExampleLayer.h"

#include "imgui/imgui.h"

#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\ext\matrix_transform.hpp>

// TEMPORARY
#include "Platform/OpenGL/OpenGLShader.h"
#include <GLFW\include\GLFW\glfw3.h>

ExampleLayer::ExampleLayer()
	: Layer("Example Layer"),
	m_CameraController(Engine::OrthographicCameraController(1280.0f / 720.0f, true)),
	m_TrianglePosition(0.0f, 0.0f, 0.0f),
	m_TriangleRotation(0.0f),
	m_ModelSpeed(2.0f),
	m_RotationSpeed(180.0f),
	m_PulseColour(0.2f, 1.0f, 0.3f, 1.0f)
{
}

void ExampleLayer::OnAttach()
{
	ENGINE_PROFILE_FUNCTION();

	// Generate and bind vertex array, vertex buffer, index buffer
	//
	// Vertex Array
	m_QuadVertexArray = Engine::VertexArray::Create();

	// Vertex Buffer
	float vertices[] = {
		//   Vertex positions   --       Colours          --    Texture coords
			0.5f,  0.5f,  0.0f,    1.0f, 0.2f, 0.2f, 1.0f,		1.0f, 1.0f,		// Top right
		   -0.5f,  0.5f,  0.0f,    0.2f, 1.0f, 0.2f, 1.0f,		0.0f, 1.0f,		// Top left
			0.5f, -0.5f,  0.0f,    0.2f, 0.2f, 1.0f, 1.0f,		1.0f, 0.0f,		// Bottom right
		   -0.5f, -0.5f,  0.0f,    0.2f, 0.2f, 0.2f, 1.0f,		0.0f, 0.0f		// Bottom left
	};
	Engine::Ref<Engine::VertexBuffer> quadVertexBuffer;
	quadVertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
	// Vertex Attribute Pointer
	Engine::BufferLayout vbLayout({
			{ Engine::ShaderDataType::Float3, "a_Position"	},
			{ Engine::ShaderDataType::Float4, "a_Colour"	},
			{ Engine::ShaderDataType::Float2, "a_TexCoords"	}
		});
	quadVertexBuffer->SetLayout(vbLayout);

	// Bind the VertexBuffer to the VertexArray
	m_QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

	// Index Buffer
	unsigned __int32 indices[] = { 0, 1, 2,
								   2, 1, 3 };
	Engine::Ref<Engine::IndexBuffer> quadIndexBuffer;
	quadIndexBuffer = Engine::IndexBuffer::Create(indices, 6);

	// Bind the IndexBuffer to the VertexArray
	m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

	// Load shader program into shader library from filepath
	m_ShaderLibrary.Load("assets/shaders/TestShader1.glsl");

	// Set m_Texture
	//m_Texture = Engine::Texture2D::Create("assets/textures/tree_render_texture.png");
	//m_Texture = Engine::Texture2D::Create("assets/textures/checkerboard.png");
	m_Texture = Engine::Texture2D::Create("assets/textures/ChernoLogo.png");
}

void ExampleLayer::OnDetach()
{
	ENGINE_PROFILE_FUNCTION();

}

void ExampleLayer::OnUpdate(Engine::Timestep deltaTime)
{
	ENGINE_PROFILE_FUNCTION();

	// ---------------------------------------------------------------------------------
	// Update section ------------------------------------------------------------------
	// ---------------------------------------------------------------------------------

	// Process any camera movement or zoom changes
	m_CameraController.OnUpdate(deltaTime);

	// IJKL-bound model matrix position movement
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::I))
		m_TrianglePosition.y += m_ModelSpeed * (float)deltaTime;
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::J))
		m_TrianglePosition.x -= m_ModelSpeed * (float)deltaTime;
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::K))
		m_TrianglePosition.y -= m_ModelSpeed * (float)deltaTime;
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::L))
		m_TrianglePosition.x += m_ModelSpeed * (float)deltaTime;
	// U and O keys to rotate
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::U))
		m_TriangleRotation += m_RotationSpeed * (float)deltaTime;
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::O))
		m_TriangleRotation -= m_RotationSpeed * (float)deltaTime;


	// ---------------------------------------------------------------------------------
	// Render section ------------------------------------------------------------------
	// ---------------------------------------------------------------------------------

	// Not clearing when above other layer (testing)
	//Engine::RenderCommand::SetClearColour(glm::vec4(0.1f, 0.2f, 0.2f, 1.0f));
	//Engine::RenderCommand::Clear();

	// Colour pulsing textured quad test
	Engine::Renderer::BeginScene(m_CameraController.GetCamera()); 
	// Get Shader from ShaderLibrary and set uniforms
	auto testShader = m_ShaderLibrary.Get("TestShader1");
	testShader->Bind();
	testShader->SetFloat4("u_PulseColour", m_PulseColour);
	testShader->SetFloat("u_Blend", (std::sin(glfwGetTime()) / 2.0f + 0.5f));
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, m_TrianglePosition);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_TriangleRotation), glm::vec3(0.3f, 0.7f, 0.4f));
	// Bind texture uniform
	m_Texture->Bind(0); // make sure this slot is the same as the int uniform set in the next line
	testShader->SetInt("u_Texture", 0);
	Engine::Renderer::Submit(testShader, m_QuadVertexArray, modelMatrix); // submit a mesh / raw vertex array, location, and material data to be rendered
	Engine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorPicker4("Pulsing colour:", glm::value_ptr(m_PulseColour));
	ImGui::End();
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	ENGINE_PROFILE_FUNCTION();

	// Pass event to camera
	m_CameraController.OnEvent(event);

	// Handle different event types
	Engine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine::WindowResizeEvent>(ENGINE_BIND_EVENT_FUNC(ExampleLayer::OnWindowResizeEvent));
}

bool ExampleLayer::OnWindowResizeEvent(Engine::WindowResizeEvent wrEvent)
{
	float currentZoom = m_CameraController.GetZoomLevel();
	m_CameraController.SetZoomLevel(currentZoom); // Can change zoom  level on resize here
	return false;
}
