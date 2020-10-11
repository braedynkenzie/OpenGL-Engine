#include "EditorLayer.h"

#include "imgui/imgui.h"
#include <glm\glm\gtc\type_ptr.hpp>

namespace Engine {

	EditorLayer::EditorLayer()
		: Layer("Editor layer"),
		m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		ENGINE_PROFILE_FUNCTION();

		// Create and bind a framebuffer
		FramebufferSpecification framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(framebufferSpec);

		m_ActiveScene = Scene::Create();
		m_QuadEntity = m_ActiveScene->CreateEntity("Quad entity");
		m_QuadEntity.AddComponent<SpriteRendererComponent>(m_QuadColour);
		/*m_ActiveScene->GetRegistry().emplace<TransformComponent>(quadEntity);
		m_ActiveScene->GetRegistry().emplace<SpriteRendererComponent>(quadEntity, m_QuadColour);*/
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep deltaTime)
	{
		ENGINE_PROFILE_FUNCTION();

		// ---------------------------------------------------------------
		// Update section ------------------------------------------------
		// ---------------------------------------------------------------

		// Process any camera movement or zoom changes
		m_CameraController.OnUpdate(deltaTime);

		// ---------------------------------------------------------------
		// Render section ------------------------------------------------
		// ---------------------------------------------------------------

		// Reset Renderer2D statistics at the start of every frame
		Renderer2D::ResetStats();
		{
			ENGINE_PROFILE_SCOPE("Render preparation");

			m_Framebuffer->Bind();

			RenderCommand::SetClearColour({ 0.2f, 0.25f, 0.25f, 1.0f });
			RenderCommand::Clear();
		}

		{
			ENGINE_PROFILE_SCOPE("Rendering / draw calls");

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			m_ActiveScene->OnUpdate(deltaTime);
			//Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 4.0f, 4.0f }, { 0.8f, 0.5f, 0.8f, 1.0f });
			Renderer2D::EndScene();

			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		ENGINE_PROFILE_FUNCTION();

		// Code adapted from imgui_demo.cpp (ShowExampleAppDockSpace())
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ImGui windows ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		ImGui::Begin("Settings");
		Renderer2D::Statistics renderStats = Renderer2D::GetStats();
		ImGui::Text("Number of draw calls per frame: %i", renderStats.DrawCalls);
		ImGui::Text("Number of quads drawn per frame: %i", renderStats.QuadCount);
		ImGui::Separator();
		ImGui::Text(m_QuadEntity.GetComponent<TagComponent>().Tag.c_str());
		glm::vec4& quadColour = m_QuadEntity.GetComponent<SpriteRendererComponent>().Colour;
		ImGui::ColorEdit4("Quad colour: ", glm::value_ptr(quadColour));
		
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (viewportSize.x != m_ViewportSize.x || viewportSize.y != m_ViewportSize.y
			&& viewportSize.x > 0 && viewportSize.y > 0)
		{
			// Viewport size has changed
			// TODO make sure m_ViewportSize is set correctly on startup so that framebuffer isn't recreated unnecessarily
			m_ViewportSize = glm::vec2(viewportSize.x, viewportSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.Resize(viewportSize.x, viewportSize.y);
		}
		// Draw from framebuffer colour attachment texture
		uint32_t viewportTextureID = m_Framebuffer->GetColourAttachment();
		ImGui::Image((void*)viewportTextureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		ENGINE_PROFILE_FUNCTION();

		m_CameraController.OnEvent(event);
	}
}