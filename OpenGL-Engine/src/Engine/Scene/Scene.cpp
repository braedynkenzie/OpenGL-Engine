#include "EnginePCH.h"

#include "Entity.h"
#include "Scene.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine {

	Entity Scene::CreateEntity(const std::string& name)
	{
		// Create the entity from a handle
		entt::entity entityHandle = m_Registry.create();
		Entity entity = Entity(entityHandle, this);
		// Add a TransformComponent to all entities
		entity.AddComponent<TransformComponent>();
		// Add a TagComponent to all entities
		auto& tagComponent = entity.AddComponent<TagComponent>(name);
		tagComponent.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep deltaTime)
	{
		// Render
		Camera* activeCamera = nullptr;
		// First, we find the main camera for the scene from one of the entities
		// Get a group/"view" of all entities will a camera component and transform component
		auto group = m_Registry.group<TransformComponent, CameraComponent>();
		for (auto entity : group)
		{
			// Get the transform (aka the model matrix) and the projection matrix of the camera component for the current entity 
			auto& [transformComponent, cameraComponent] = group.get<TransformComponent, CameraComponent>(entity);

			if (cameraComponent.IsPrimaryCamera)
			{
				activeCamera = &cameraComponent.Camera;
				break;
			}
		}

		// Now we should have the active camera to render with
		if (activeCamera != nullptr)
		{
			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto& [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transformComponent.Transform, spriteComponent.Colour);
			}
		}


	}
}
