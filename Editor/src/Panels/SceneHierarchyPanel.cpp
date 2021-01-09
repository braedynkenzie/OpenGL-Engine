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
		ImGuiTreeNodeFlags flags = ((entity == m_SelectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
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
				ImGui::DragFloat3("Position", glm::value_ptr(entityTransform[3]), 0.005f);
				ImGui::TreePop();
			}
		}

		if (selectedEntity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera component"))
			{
				CameraComponent& cameraComponent = selectedEntity.GetComponent<CameraComponent>();
				SceneCamera& camera = cameraComponent.Camera;

				if (ImGui::Checkbox("Primary camera", &cameraComponent.IsPrimaryCamera))
				{
					// Go through all other entities with a camera component and update the old primary camera to no longer be
					m_ContextScene->m_Registry.each([&](auto entityID) {
						Entity entity = Entity{ entityID, m_ContextScene.get() };
						if (entity.HasComponent<CameraComponent>() && entity != selectedEntity)
							entity.GetComponent<CameraComponent>().IsPrimaryCamera = false;
					});
				}

				const char* projTypeLabels[] = { "Perspective", "Orthographic" };
				const char* currentProjType = projTypeLabels[(uint32_t)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection type", currentProjType))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = (currentProjType == projTypeLabels[i]);
						if (ImGui::Selectable(projTypeLabels[i], isSelected)) 
						{
							currentProjType = projTypeLabels[i];
							camera.SetProjectionType(i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveFOVyDegrees = glm::degrees(camera.GetPerspectiveFOVy());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveFOVyDegrees, 0.1f, 0.0f, 180.0f))
						camera.SetPerspectiveFOVy(glm::radians(perspectiveFOVyDegrees));

					float perspectiveNearBound = camera.GetPerspectiveNearBound();
					float perspectiveFarBound = camera.GetPerspectiveFarBound();
					if (ImGui::DragFloat("Near clip", &perspectiveNearBound, 0.01f, 0.0f, perspectiveFarBound))
						camera.SetPerspectiveNearBound(perspectiveNearBound);
					if (ImGui::DragFloat("Far clip", &perspectiveFarBound, 1.0f, perspectiveNearBound, 1000000.0f))
						camera.SetPerspectiveFarBound(perspectiveFarBound);
				}
				else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					ImGui::Checkbox("Fixed aspect ratio", &cameraComponent.HasFixedAspectRatio);

					float orthographicSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthographicSize, 0.4f))
						camera.SetOrthographicSize(orthographicSize);

					float orthographicNearBound = camera.GetOrthographicNearBound();
					if (ImGui::DragFloat("Near clip", &orthographicNearBound, 0.2f))
						camera.SetOrthographicNearBound(orthographicNearBound);

					float orthographicFarBound = camera.GetOrthographicFarBound();
					if (ImGui::DragFloat("Far clip", &orthographicFarBound, 0.2f))
						camera.SetOrthographicFarBound(orthographicFarBound);
				}

				ImGui::TreePop();
			}

		}

		
		if (selectedEntity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer component"))
			{
				SpriteRendererComponent& spriteComponent = selectedEntity.GetComponent<SpriteRendererComponent>();
				glm::vec4& colour = spriteComponent.Colour;
				ImGui::ColorEdit4("Colour", glm::value_ptr(colour));
				ImGui::TreePop();
			}
		}
	}

}