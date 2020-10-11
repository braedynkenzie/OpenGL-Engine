#include "EnginePCH.h"

#include "Entity.h"
#include "Scene.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine {

	Entity Scene::CreateEntity(const std::string& name)
	{
		entt::entity entityHandle = m_Registry.create();
		Entity entity = Entity(entityHandle, this);
		entity.AddComponent<TransformComponent>();
		auto& tagComponent = entity.AddComponent<TagComponent>(name);
		tagComponent.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep deltaTime)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [modelMatrix, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(modelMatrix, sprite.Colour);
		}
	}
}
