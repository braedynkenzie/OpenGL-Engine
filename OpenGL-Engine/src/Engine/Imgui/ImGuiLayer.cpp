#include "EnginePCH.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

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
		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enables keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enables docking of ImGui UI components
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enables Multi-Viewports (on Windows platform)
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enables gamepad controls
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge; 

		// Setup ImGui style
		ImGui::StyleColorsDark();

		// Tweak window rounding and background when viewports are enabled
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Get the application's window
		Application& app = Application::GetInstance();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Renderer / Platform bindings
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		Window& window = app.GetWindow();

		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

		// ImGui rendering calls
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}
	}

}