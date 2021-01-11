#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Engine {

	class Entity
	{ 
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scenePtr);
		Entity(const Entity& otherEntity) = default;
		~Entity() = default;

		template<typename T>
		T& GetComponent()
		{
			ENGINE_CORE_ASSERT(HasComponent<T>(), "Entity does not have this type of component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ENGINE_CORE_ASSERT(!HasComponent<T>(), "Trying to add a component that this Entity already has!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			ENGINE_CORE_ASSERT(HasComponent<T>(), "Entity does not have this type of component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() 
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		// Operator overloads
		bool operator==(const Entity& otherEntity) {
			return (this->m_EntityHandle == otherEntity.m_EntityHandle)
				&& (this->m_Scene == otherEntity.m_Scene);
		}
		bool operator!=(const Entity& otherEntity) {
			return !operator==(otherEntity);
		}
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return (entt::entity)m_EntityHandle; }
		operator bool() const { return (uint32_t)m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr; // TODO might want to make this a weak ptr
	};

}
