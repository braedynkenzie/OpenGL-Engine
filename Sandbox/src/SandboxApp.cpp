
#include "Engine.h"

#include "imgui/imgui.h"

#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\ext\matrix_transform.hpp>

// TEMPORARY
#include "Platform/OpenGL/OpenGLShader.h"
#include "Engine/Renderer/Shader.h"
#include <GLFW\include\GLFW\glfw3.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"),
		m_CameraController(Engine::OrthographicCameraController(1280.0f / 720.0f, true)),
		m_TrianglePosition(0.0f, 0.0f, 0.0f),
		m_TriangleRotation(0.0f),
		m_ModelSpeed(2.0f),
		m_RotationSpeed(180.0f),
		m_PulseColour(glm::vec4(0.2f, 1.0f, 0.3f, 1.0f))
	{

		// Generate and bind vertex array, vertex buffer, index buffer
		//
		// Vertex Array
		m_QuadVertexArray.reset(Engine::VertexArray::Create());

		// Vertex Buffer
		float vertices[] = {
			//   Vertex positions   --       Colours          --    Texture coords
				0.5f,  0.5f,  0.0f,    1.0f, 0.2f, 0.2f, 1.0f,		1.0f, 1.0f,		// Top right
			   -0.5f,  0.5f,  0.0f,    0.2f, 1.0f, 0.2f, 1.0f,		0.0f, 1.0f,		// Top left
				0.5f, -0.5f,  0.0f,    0.2f, 0.2f, 1.0f, 1.0f,		1.0f, 0.0f,		// Bottom right
			   -0.5f, -0.5f,  0.0f,    0.2f, 0.2f, 0.2f, 1.0f,		0.0f, 0.0f		// Bottom left
		};
		Engine::Ref<Engine::VertexBuffer> quadVertexBuffer;
		quadVertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
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
		unsigned __int32 indices[] =  { 0, 1, 2, 
										2, 1, 3 };
		Engine::Ref<Engine::IndexBuffer> quadIndexBuffer;
		quadIndexBuffer.reset(Engine::IndexBuffer::Create(indices, 6));

		// Bind the IndexBuffer to the VertexArray
		m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		// Load shader program into shader library from filepath
		m_ShaderLibrary.Load("assets/shaders/TexturedQuad.glsl");

		// Set m_Texture
		//m_Texture = Engine::Texture2D::Create("assets/textures/tree_render_texture.png");
		//m_Texture = Engine::Texture2D::Create("assets/textures/checkerboard.png");
		m_Texture = Engine::Texture2D::Create("assets/textures/ChernoLogo.png");
	}

	void OnUpdate(Engine::Timestep deltaTime) override
	{
		// ---------------------------------------------------------------------------------
		// Update section ------------------------------------------------------------------
		// ---------------------------------------------------------------------------------

		// Process any camera movement or zoom changes
		m_CameraController.OnUpdate(deltaTime);

		// IJKL-bound model matrix position movement
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_I))
			m_TrianglePosition.y += m_ModelSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_J))
			m_TrianglePosition.x -= m_ModelSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_K))
			m_TrianglePosition.y -= m_ModelSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_L))
			m_TrianglePosition.x += m_ModelSpeed * (float)deltaTime;
		// U and O keys to rotate triangle
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_U))
			m_TriangleRotation += m_RotationSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_O))
			m_TriangleRotation -= m_RotationSpeed * (float)deltaTime;


		// ---------------------------------------------------------------------------------
		// Render section ------------------------------------------------------------------
		// ---------------------------------------------------------------------------------

		Engine::RenderCommand::SetClearColour(glm::vec4(0.1, 0.2, 0.2, 1.0));
		Engine::RenderCommand::Clear();

		// Hello Triangle
		Engine::Renderer::BeginScene(m_CameraController.GetCamera()); // TODO also pass lights, environment
		// Get Shader from ShaderLibrary and set uniforms
		auto TexturedQuadShader = m_ShaderLibrary.Get("TexturedQuad");
		std::dynamic_pointer_cast<Engine::OpenGLShader>(TexturedQuadShader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(TexturedQuadShader)->UploadUniformFloat4("u_PulseColour", m_PulseColour);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(TexturedQuadShader)->UploadUniformFloat1("u_Blend", (std::sin(glfwGetTime()) / 2.0f + 0.5f));
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_TrianglePosition);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_TriangleRotation), glm::vec3(0.3f, 0.7f, 0.4f));
		// Bind texture uniform
		m_Texture->Bind(0); // make sure this slot is the same as the int uniform set in the next line
		std::dynamic_pointer_cast<Engine::OpenGLShader>(TexturedQuadShader)->UploadUniformInt("u_Texture", 0);
		Engine::Renderer::Submit(TexturedQuadShader, m_QuadVertexArray, modelMatrix); // submit a mesh / raw vertex array, location, and material data to be rendered
		Engine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorPicker4("Pulsing colour:", glm::value_ptr(m_PulseColour));
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
		// Pass event to camera
		m_CameraController.OnEvent(event);

		// Example input using the Engine's event system
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::KeyPressEvent>(ENGINE_BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Engine::KeyPressEvent kpEvent)
	{
		// Example code 
		return false;
	}

private:
	Engine::OrthographicCameraController m_CameraController;
	glm::vec3 m_TrianglePosition;
	float m_TriangleRotation;
	float m_CameraSpeed;
	float m_ModelSpeed;
	float m_RotationSpeed;
	glm::vec4 m_PulseColour;
	Engine::Ref<Engine::VertexArray> m_QuadVertexArray;
	Engine::Ref<Engine::Texture2D> m_Texture;
	// TEMPORARY
	//Engine::Ref<Engine::Shader> m_Shader;
	Engine::ShaderLibrary m_ShaderLibrary;

};

class Sandbox : public Engine::Application 
{
public:
	Sandbox()
	{
		this->PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}
