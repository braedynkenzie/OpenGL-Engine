#include "SceneHierarchyPanel.h"

#include <imgui\imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
			DrawEntityTreeNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity = {};
		}

		ImGui::End();

		// TODO will be refactoring the properties panel into its own class likely
		ImGui::Begin("Properties");

		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityTreeNode(Entity entity)
	{
		std::string& entityTag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((entity == m_SelectedEntity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow;
		bool isExpanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityTag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}
		if (isExpanded)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity selectedEntity)
	{
		if (selectedEntity.HasComponent<TagComponent>())
		{
			std::string& entityTag = selectedEntity.GetComponent<TagComponent>().Tag;

			// Create a char buffer on the stack to store an input for a new tag component
			char charBuffer[256];
			memset(charBuffer, 0, sizeof(charBuffer));
			strcpy_s(charBuffer, sizeof(charBuffer), entityTag.c_str());
			if (ImGui::InputText("Tag component", charBuffer, sizeof(charBuffer))) 
			{
				entityTag = std::string(charBuffer);
			}
		}

		if (selectedEntity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform component"))
			{
				glm::mat4& entityTransform = selectedEntity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(entityTransform[3]), 0.1f);
				ImGui::TreePop();
			}
		}

		if (selectedEntity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera component"))
			{
				CameraComponent& cameraComponent = selectedEntity.GetComponent<CameraComponent>();

				//ImGui::BeginCombo("Projection type", ...)

				ImGui::TreePop();
			}

		}
	}

}