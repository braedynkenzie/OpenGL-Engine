#pragma once

#include "entt.hpp"
#include "Engine\Core\Timestep.h"
#include "Engine\Core\Core.h"

namespace Engine {

	class Entity;

	class Scene 
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Create() { return CreateRef<Scene>(); }

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep deltaTime);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		friend class SceneHierarchyPanel;
		friend class Entity;
		friend class SceneSerializer;

		// Registry to contain the component data and entity IDs
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
	};

}