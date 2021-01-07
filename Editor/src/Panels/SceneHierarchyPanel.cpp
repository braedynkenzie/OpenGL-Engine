#include "SceneHierarchyPanel.h"

#include <imgui\imgui.h>

namespace Engine {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		m_ContextScene = scene;
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_ContextScene = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy Panel");

		m_ContextScene->m_Registry.each([&](auto entityID) {
			Entity entity = Entity{ entityID, m_ContextScene.get() };
			TagComponent& entityTag = entity.GetComponent<TagComponent>();
			ImGui::Text(entityTag.Tag.c_str());
		});

		ImGui::End();
	}

}