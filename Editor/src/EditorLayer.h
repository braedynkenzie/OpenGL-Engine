#pragma once

#include "Engine.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Engine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep deltaTime) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = glm::vec2(0.0f, 0.0f);

		Ref<Scene> m_ActiveScene;
		
		// TEMPORARY
		Entity m_GreenQuadEntity;
		Entity m_BlueQuadEntity;
		glm::vec4 m_GreenQuadColour = glm::vec4(0.1f, 0.8f, 0.1f, 0.5f);
		glm::vec4 m_BlueQuadColour = glm::vec4(0.1f, 0.1f, 0.8f, 1.0f);
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;
		bool firstCamActive = true;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		// Saving/Loading scenes
		Engine::Ref<SceneSerializer> m_Serializer;
	};
}
