
#include "Engine.h"
#include "imgui/imgui.h"

#include <glm\glm\ext\matrix_transform.hpp>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), 
		m_Camera(Engine::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f)), 
		m_CameraPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_TrianglePosition(0.0f, 0.0f, 0.0f),
		m_TriangleRotation(0.0f),
		m_CameraSpeed(0.02f * 60),
		m_ModelSpeed(0.02f * 60),
		m_RotationSpeed(1.4 * 60)
	{
		m_Camera.SetPosition(m_CameraPosition);

		// Generate and bind vertex array, vertex buffer, index buffer
		//
		// Vertex Array
		m_VertexArray.reset(Engine::VertexArray::Create());

		// Vertex Buffer
		float vertices[] = {
			//   Vertex positions   --    Colours
				0.5f,  0.5f,  0.0f,    1.0f, 0.2f, 0.2f, 1.0f,
			   -0.5f,  0.5f,  0.0f,    0.2f, 1.0f, 0.2f, 1.0f,
				0.0f, -0.5f,  0.0f,    0.2f, 0.2f, 1.0f, 1.0f,
		};
		std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
		// Vertex Attribute Pointer
		Engine::BufferLayout vbLayout({
				{Engine::ShaderDataType::Float3, "a_Position"},
				{Engine::ShaderDataType::Float4, "a_Colour"}
			});
		vertexBuffer->SetLayout(vbLayout);

		// Bind the VertexBuffer to the VertexArray
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		unsigned __int32 indices[] = { 0, 1 ,2 };
		std::shared_ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create(indices, 3));

		// Bind the IndexBuffer to the VertexArray
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Hello Triangle first shader program
		std::string vertexSourceCode = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			//uniform mat4 u_ViewMatrix;
			//uniform mat4 u_ProjectionMatrix;
			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_ModelMatrix;

			out vec4 v_Colour; 
			
			void main() {
				v_Colour = a_Colour;
				gl_Position =  u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSourceCode = R"(
			#version 330 core

			in vec4 v_Colour;
			out vec4 FragColour;
			
			void main() {
				FragColour = v_Colour;
			}
		)";

		// Set m_Shader
		m_Shader.reset(new Engine::Shader(vertexSourceCode, fragmentSourceCode));
	}

	void OnUpdate(Engine::Timestep deltaTime) override
	{
		// Testing timesteps
		// ENGINE_TRACE("Delta time from Example layer: {0} seconds, {1} milliseconds", deltaTime.GetSeconds(), deltaTime.GetMilliseconds());

		// WASD-bound camera movement
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_W))
			m_CameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
			m_CameraPosition -= glm::vec3(1.0f, 0.0f, 0.0f) * m_CameraSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_S))
			m_CameraPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
			m_CameraPosition += glm::vec3(1.0f, 0.0f, 0.0f) * m_CameraSpeed * (float)deltaTime;
		// E and Q keys to rotate camera
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_E))
			m_Camera.SetZRotation(m_Camera.GetZRotation() - (m_RotationSpeed * (float)deltaTime));
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_Q))
			m_Camera.SetZRotation(m_Camera.GetZRotation() + (m_RotationSpeed * (float)deltaTime));
		// IJKL-bound model matrix position movement
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_I))
			m_TrianglePosition += glm::vec3(0.0f, 1.0f, 0.0f) * m_ModelSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_J))
			m_TrianglePosition -= glm::vec3(1.0f, 0.0f, 0.0f) * m_ModelSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_K))
			m_TrianglePosition -= glm::vec3(0.0f, 1.0f, 0.0f) * m_ModelSpeed * (float)deltaTime;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_L))
			m_TrianglePosition += glm::vec3(1.0f, 0.0f, 0.0f) * m_ModelSpeed * (float)deltaTime;
		// U and O keys to rotate triangle
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_U))
			m_TriangleRotation += (m_RotationSpeed * (float)deltaTime);
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_O))
			m_TriangleRotation -= (m_RotationSpeed * (float)deltaTime);

		Engine::RenderCommand::SetClearColour(glm::vec4(0.1, 0.2, 0.2, 1.0));
		Engine::RenderCommand::Clear();

		// Hello Triangle
		m_Camera.SetPosition(m_CameraPosition);
		Engine::Renderer::BeginScene(m_Camera); // TODO also pass lights, environment
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_TrianglePosition);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_TriangleRotation), glm::vec3(0.3f, 0.7f, 0.4f));
		Engine::Renderer::Submit(m_Shader, m_VertexArray, modelMatrix); // submit a mesh / raw vertex array, location, and material data to be rendered
		Engine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
		// Example input using the Engine's event system
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::KeyPressEvent>(ENGINE_BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Engine::KeyPressEvent kpEvent)
	{
		if (kpEvent.GetKeyCode() == ENGINE_KEY_UP)
			m_CameraPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraSpeed;
		if (kpEvent.GetKeyCode() == ENGINE_KEY_LEFT)
			m_CameraPosition -= glm::vec3(-1.0f, 0.0f, 0.0f) * m_CameraSpeed;
		if (kpEvent.GetKeyCode() == ENGINE_KEY_DOWN)
			m_CameraPosition -= glm::vec3(0.0f, -1.0f, 0.0f) * m_CameraSpeed;
		if (kpEvent.GetKeyCode() == ENGINE_KEY_RIGHT)
			m_CameraPosition -= glm::vec3(1.0f, 0.0f, 0.0f) * m_CameraSpeed;
		return false;
	}

private:
	Engine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	glm::vec3 m_TrianglePosition;
	float m_TriangleRotation;
	float m_CameraSpeed;
	float m_ModelSpeed;
	float m_RotationSpeed;
	std::shared_ptr<Engine::VertexArray> m_VertexArray;
	// TEMPORARY
	std::shared_ptr<Engine::Shader> m_Shader;

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
