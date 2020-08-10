#include "EnginePCH.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform\OpenGL\ImGuiOpenGLRenderer.h"

#include "Engine\Application.h"

// Temporary includes until OpenGL-Engine has ImGui fully integrated
#include <GLFW\glfw3.h>
#include <glad/glad.h>

namespace Engine {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Temporary: TODO use own OpenGL-Engine keycodes
		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		// Set ImGui display size and delta time
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		Window& window = app.GetWindow();
		io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());
		float currentTime = (float)glfwGetTime();
		float deltaTime = m_Time > 0.0f ? (currentTime - m_Time) : (1.0f / 60.0f);
		io.DeltaTime = deltaTime;
		m_Time = currentTime;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);

		// ImGui draw calls
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		ENGINE_TRACE("Event passed to ImGui Layer: {0}", event);

		EventDispatcher eventDispatcher(event);
		// Bind event types to class methods
		// NOTE: Could do this with lambdas instead of "std::bind"-ing class methods
		eventDispatcher.Dispatch<MouseMoveEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnMouseMoveEvent));
		eventDispatcher.Dispatch<MouseScrollEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnMouseScrollEvent));
		eventDispatcher.Dispatch<MouseButtonPressEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnMouseButtonPressEvent));
		eventDispatcher.Dispatch<MouseButtonReleaseEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnMouseButtonReleaseEvent));
		eventDispatcher.Dispatch<KeyPressEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnKeyPressEvent));
		eventDispatcher.Dispatch<KeyReleaseEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnKeyReleaseEvent));
		eventDispatcher.Dispatch<KeyTypedEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnKeyTypedEvent));
		eventDispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FUNC(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& mmEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(mmEvent.GetX(), mmEvent.GetY());

		return false; // event not set as handled
	}

	bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& msEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		// Vertical scroll
		io.MouseWheel += msEvent.GetYOffset();
		// Horizontal scroll
		io.MouseWheelH += msEvent.GetXOffset();

		return false; // event not set as handled
	}

	bool ImGuiLayer::OnMouseButtonPressEvent(MouseButtonPressEvent& mbpEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[mbpEvent.GetMouseButton()] = true;

		return false; // event not set as handled
	}

	bool ImGuiLayer::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& mbrEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[mbrEvent.GetMouseButton()] = false;

		return false; // event not set as handled
	}

	bool ImGuiLayer::OnKeyPressEvent(KeyPressEvent& kpEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[kpEvent.GetKeyCode()] = true;

		// Check modifier keys
		io.KeyCtrl	= io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyAlt	= io.KeysDown[GLFW_KEY_LEFT_ALT]	 || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT]	 || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]	 || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false; // event not set as handled
	}

	bool ImGuiLayer::OnKeyReleaseEvent(KeyReleaseEvent& krEvent)
	{
		return false; // event not set as handled
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& ktEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keyCode= ktEvent.GetKeyCode();
		if (keyCode > 0 && keyCode < 0x10000)
		{
			io.AddInputCharacter((unsigned short)keyCode);
		}
		
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& wrEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(wrEvent.GetWidth(), wrEvent.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, wrEvent.GetWidth(), wrEvent.GetHeight());

		return false; // event not set as handled
	}

}