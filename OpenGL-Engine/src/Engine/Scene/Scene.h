#pragma once

#include "entt.hpp"

namespace Engine {

	class Scene 
	{
	public:
		Scene();
		~Scene();

	private:
		// Registry to contain the component data and entity IDs
		entt::registry m_Registry;

	};

}