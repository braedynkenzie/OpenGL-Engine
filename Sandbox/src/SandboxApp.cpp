
#include "Engine.h"
#include "imgui/imgui.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), 
		m_Camera(Engine::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f)), 
		m_CameraPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_CameraSpeed(0.01f)
	{
		m_Camera.SetPosition(m_CameraPosition);

		// Generate and bind vertex array, vertex buffer, index buffer
		//
		// Vertex Array
		//m_VertexArray = std::make_shared<VertexArray>(VertexArray::Create());
		m_VertexArray.reset(Engine::VertexArray::Create());

		// Vertex Buffer
		float vertices[] = {
			//   Vertex positions   --    Colours
				0.5f,  0.5f,  0.0f,    1.0f, 0.2f, 0.2f, 1.0f,
			   -0.5f,  0.5f,  0.0f,    0.2f, 1.0f, 0.2f, 1.0f,
				0.0f, -0.5f,  0.0f,    0.2f, 0.2f, 1.0f, 1.0f,
		};
		//m_VertexBuffer = std::make_shared<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
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
		//m_IndexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::Create(indices, 3));
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

			out vec4 v_Colour; 
			
			void main() {
				v_Colour = a_Colour;
				//gl_Position = vec4(a_Position, 1.0);
				//gl_Position =  u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
				gl_Position =  u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSourceCode = R"(
			#version 330 core

			in vec4 v_Colour;
			out vec4 FragColour;
			
			void main() {
				//FragColour = vec4(0.2, 1.0, 0.6, 1.0);
				FragColour = v_Colour;
			}
		)";

		// Set m_Shader
		//m_Shader = std::make_shared<Shader>(Shader(vertexSourceCode, fragmentSourceCode));
		m_Shader.reset(new Engine::Shader(vertexSourceCode, fragmentSourceCode));
	}

	void OnUpdate() override
	{
		// WASD-bound camera movement
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_W))
			m_CameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraSpeed;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
			m_CameraPosition -= glm::vec3(1.0f, 0.0f, 0.0f) * m_CameraSpeed;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_S))
			m_CameraPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraSpeed;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
			m_CameraPosition += glm::vec3(1.0f, 0.0f, 0.0f) * m_CameraSpeed;
		// E and Q keys to rotate camera
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_E))
			m_Camera.SetZRotation(m_Camera.GetZRotation() - 0.4);
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_Q))
			m_Camera.SetZRotation(m_Camera.GetZRotation() + 0.4);

		Engine::RenderCommand::SetClearColour(glm::vec4(0.1, 0.2, 0.2, 1.0));
		Engine::RenderCommand::Clear();

		// Hello Triangle
		m_Camera.SetPosition(m_CameraPosition);
		Engine::Renderer::BeginScene(m_Camera); // TODO also pass lights, environment
		Engine::Renderer::Submit(m_Shader, m_VertexArray); // submit a mesh / raw vertex array and material data
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
	float m_CameraSpeed;
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
