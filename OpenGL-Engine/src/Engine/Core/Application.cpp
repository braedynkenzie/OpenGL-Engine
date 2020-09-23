#include "EnginePCH.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Renderer.h"

// TEMPORARY 
// For glfwGetTime() 
#include <GLFW\glfw3.h>

namespace Engine {

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		ENGINE_PROFILE_FUNCTION();

		// Assert that no previous instance of Application has been created
		ENGINE_CORE_ASSERT(!s_Instance, "Application (singleton) already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProperties(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		// Initialize the renderer (ie. setup blending, depth testing, etc)
		Renderer::Init();

		// Create and push an ImGui overlay
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		ENGINE_PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		while (m_Running) {
			ENGINE_PROFILE_SCOPE("Run Loop in Application::Run()");

			float currentTime = (float)glfwGetTime(); // TODO make Platform::GetTime() implementation
			Timestep deltaTime(currentTime - m_LastFrameTime);
			m_LastFrameTime = currentTime;

			if (!m_Minimized)
			{
				{
					ENGINE_PROFILE_SCOPE("All game layer updates");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(deltaTime);
				}
			}

			m_ImGuiLayer->Begin();
			{
				ENGINE_PROFILE_SCOPE("All ImGui layer updates");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	// OnEvent is bound as the window event callback in the application constructor (above)
	void Application::OnEvent(Event& event)
	{
		// Log the event
		ENGINE_CORE_TRACE("{0}", event);

		// Pass the event through all layers until handled
		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin(); )
		{
			(*--iterator)->OnEvent(event);
			if (event.Handled)
				break;
		}

		// Handle specific events using EventDispatcher
		EventDispatcher eventDispatcher = EventDispatcher(event);
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
		eventDispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResizeEvent));
	}

	void Application::PushLayer(Layer* layer)
	{
		ENGINE_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		ENGINE_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& wcEvent)
	{
		ENGINE_PROFILE_FUNCTION();

		m_Running = false;
		return true; // handled 
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& wrEvent)
	{
		ENGINE_PROFILE_FUNCTION();

		if (wrEvent.GetWidth() == 0 || wrEvent.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		
		// Resize framebuffers
		Renderer::OnWindowResize(wrEvent.GetWidth(), wrEvent.GetHeight());

		return false;
	}
}