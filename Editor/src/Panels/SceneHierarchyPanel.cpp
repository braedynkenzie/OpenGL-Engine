#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

		// Right click (on empty space) menu 
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create new entity"))
				m_ContextScene->CreateEntity("New Entity");
			ImGui::EndPopup();
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
			m_SelectedEntity = entity;

		// Right click menu for deleting entities
		bool deferredEntityDeletion = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete entity"))
				deferredEntityDeletion = true;
			ImGui::EndPopup();
		}
		if (isExpanded)
		{
			ImGui::TreePop();
		}

		// Defer entity deletion to be the last thing we do (if it's required)
		if (deferredEntityDeletion)
		{
			m_ContextScene->DestroyEntity(m_SelectedEntity);
			if (entity == m_SelectedEntity)
				m_SelectedEntity = {};
		}

	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetToValue, float firstColumnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		// The following is borrowed and adapted from the ImGui example code for their DragFloat UI function
		// First we want two colomns so that our label can be on the left
		ImGui::Columns(2);
		// Want a fixed width for our labels column
		ImGui::SetColumnWidth(0, firstColumnWidth);
		ImGui::Text(label.c_str());
		// Figure out sizing details for the other column's elements
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		// Set colours and display the three elements in the values vector parameter
		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.95f, 0.25f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetToValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.9f, 0.4f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetToValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2, 0.4f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetToValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();

		ImGui::Columns(1);
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
				entityTag = std::string(charBuffer);
		}

		if (selectedEntity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform component"))
			{
				TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", transformComponent.Translation, 0.0f);
				glm::vec3 rotationDegrees = glm::degrees(transformComponent.Rotation);
				DrawVec3Control("Rotation", rotationDegrees, 0.0f);
				transformComponent.Rotation = glm::radians(rotationDegrees);
				DrawVec3Control("Scale", transformComponent.Scale, 1.0f);
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