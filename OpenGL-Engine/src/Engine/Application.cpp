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
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);
		// Vertex Buffer and Vertex Attribute Pointer
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		float vertices[] = {
			0.5f,  0.5f,  0.0f,
		   -0.5f,  0.5f,  0.0f,
			0.0f, -0.5f,  0.0f,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, nullptr);
		// Index Buffer
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		unsigned int indices[] = {
			0, 1 ,2
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// TESTING -- first shader program
		std::string vertexSourceCode = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			
			void main() {
				gl_Position = vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSourceCode = R"(
			#version 330 core

			out vec4 FragColour;
			
			void main() {
				FragColour = vec4(0.2, 1.0, 0.6, 1.0);
			}
		)"; 

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

			// TESTING HELLO TRIANGLE
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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