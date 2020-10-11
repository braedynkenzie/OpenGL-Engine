#pragma once

#include "entt.hpp"
#include "Engine\Core\Timestep.h"

namespace Engine {

	class Entity;

	class Scene 
	{
	public:
		Scene() = default;
		~Scene() = default;

		static Ref<Scene> Create() { return CreateRef<Scene>(); }

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep deltaTime);

	private:
		// Registry to contain the component data and entity IDs
		entt::registry m_Registry;

		friend class Entity;

	};

}