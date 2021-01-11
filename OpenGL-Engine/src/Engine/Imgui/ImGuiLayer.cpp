#include "EnginePCH.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Engine/Core/Application.h"

// Temporary includes until OpenGL-Engine has ImGui fully integrated
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{
		ENGINE_PROFILE_FUNCTION();

	}

	ImGuiLayer::~ImGuiLayer()
	{
		ENGINE_PROFILE_FUNCTION();

	}

	void ImGuiLayer::OnAttach()
	{
		ENGINE_PROFILE_FUNCTION();

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

		// Add custom font
		io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans/WorkSans-Bold.ttf", 16.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans/WorkSans-Regular.ttf", 16.0f);
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);

		// Setup ImGui style
		ImGui::StyleColorsDark();

		// Tweak window rounding and background when viewports are enabled
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkTheme();

		// Get the application's window
		Application& app = Application::GetInstance();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Renderer / Platform bindings
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::OnDetach()
	{
		ENGINE_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ENGINE_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ENGINE_PROFILE_FUNCTION();

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

	void ImGuiLayer::SetDarkTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colours = style.Colors;
		colours[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// TODO choose different colours
		// These are currently borrowed from https://github.com/TheCherno/Hazel/commit/aab43e62129abc869d00ee508b4467bbe1c4f319
		// Headers
		colours[ImGuiCol_Header]		= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colours[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colours[ImGuiCol_HeaderActive]	= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		// Buttons
		colours[ImGuiCol_Button]		= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colours[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colours[ImGuiCol_ButtonActive]	= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		// Frame BG
		colours[ImGuiCol_FrameBg]		= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colours[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colours[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		// Tabs
		colours[ImGuiCol_Tab]			= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TabHovered]	= ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colours[ImGuiCol_TabActive]		= ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colours[ImGuiCol_TabUnfocused]	= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		// Title
		colours[ImGuiCol_TitleBg]			= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TitleBgActive]		= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TitleBgCollapsed]	= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	}

}