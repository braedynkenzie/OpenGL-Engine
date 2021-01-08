#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"

namespace Engine {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();

	private:
		void DrawEntityTreeNode(Entity entity);
		void DrawComponents(Entity selectedEntity);

	private:
		Ref<Scene> m_ContextScene;
		Entity m_SelectedEntity;
	};

}