#include "EnginePCH.h"

#include "Scene.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine {

	Engine::Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		// TODO
		entt::entity entity = m_Registry.create();
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
