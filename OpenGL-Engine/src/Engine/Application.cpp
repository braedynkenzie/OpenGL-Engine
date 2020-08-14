#include "EnginePCH.h"
#include "Engine/Application.h"
#include "Engine/Log.h"
#include "Engine/Input.h"

#include <glad/glad.h>

namespace Engine {

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Assert that no previous instance of Application has been created
		ENGINE_CORE_ASSERT(!s_Instance, "Application (singleton) already exists!");
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		// Create and push an ImGui overlay
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// TESTING OPENGL -- HELLO TRIANGLE
		// Generate and bind vertex array, vertex buffer, index buffer
		//
		// Vertex Array
		//m_VertexArray = std::make_shared<VertexArray>(VertexArray::Create());
		m_VertexArray.reset(VertexArray::Create());

		// Vertex Buffer
		float vertices[] = {
			//   Vertex positions   --    Colours
				0.5f,  0.5f,  0.0f,    1.0f, 0.2f, 0.2f, 1.0f,
			   -0.5f,  0.5f,  0.0f,    0.2f, 1.0f, 0.2f, 1.0f,
				0.0f, -0.5f,  0.0f,    0.2f, 0.2f, 1.0f, 1.0f,
		};
		//m_VertexBuffer = std::make_shared<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		// Vertex Attribute Pointer
		BufferLayout vbLayout({
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Colour"}
			});
		vertexBuffer->SetLayout(vbLayout);

		// Bind the VertexBuffer to the VertexArray
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		unsigned __int32 indices[] = { 0, 1 ,2 };
		//m_IndexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::Create(indices, 3));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3));

		// Bind the IndexBuffer to the VertexArray
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Hello Triangle first shader program
		std::string vertexSourceCode = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			out vec4 v_Colour; 
			
			void main() {
				v_Colour = a_Colour;
				gl_Position = vec4(a_Position, 1.0);
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
		m_Shader.reset(new Shader(vertexSourceCode, fragmentSourceCode));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {

			glClearColor(0.1, 0.2, 0.2, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			// Hello Triangle
			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			// Testing input polling
			//auto[x,y] = Input::GetMousePosition();
			//ENGINE_CORE_TRACE("{0}, {1}", x, y);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	// OnEvent is bound as the window event callback in the application constructor (above)
	void Application::OnEvent(Event& event)
	{
		// Log the event
		ENGINE_CORE_TRACE("{0}", event);

		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin(); )
		{
			(*--iterator)->OnEvent(event);
			if (event.Handled)
				break;
		}
		
		// Handle events using EventDispatcher
		EventDispatcher eventDispatcher = EventDispatcher(event);
		// Bind WindowCloseEvents to OnWindowCloseEvent method
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent)); 
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& WCEvent)
	{
		m_Running = false;
		return true;
	}
}