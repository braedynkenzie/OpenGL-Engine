#pragma once

#include "Engine.h"

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
		entt::entity m_QuadEntity;
		glm::vec4 m_QuadColour = glm::vec4(0.1f, 0.8f, 0.1f, 1.0f);

	};
}
