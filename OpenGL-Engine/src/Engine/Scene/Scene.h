#pragma once

#include "entt.hpp"
#include "Engine\Core\Timestep.h"

namespace Engine {

	class Scene 
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(Timestep deltaTime);

		// TEMPORARY
		entt::registry& GetRegistry() { return m_Registry; }

	private:
		// Registry to contain the component data and entity IDs
		entt::registry m_Registry;

	};

}