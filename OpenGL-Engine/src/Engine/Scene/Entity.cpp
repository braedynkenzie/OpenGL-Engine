#include "EnginePCH.h"
#include "Entity.h"

namespace Engine {

	Entity::Entity(entt::entity entityHandle, Scene* scenePtr)
		: m_EntityHandle(entityHandle),
		m_Scene(scenePtr) {}

}