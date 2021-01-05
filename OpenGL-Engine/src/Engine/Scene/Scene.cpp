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
		glm::mat4* cameraTransform = nullptr;

		// First, we find the main camera for the scene from one of the entities
		// Get a group/"view" of all entities will a camera component and transform component
		{
			auto entityView = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : entityView)
			{
				// Get the camera's transform (ie its uninverted view matrix) and the camera component (containing the projection matrix)
				auto& [transformComponent, cameraComponent] = entityView.get<TransformComponent, CameraComponent>(entity);

				if (cameraComponent.IsPrimaryCamera)
				{
					activeCamera = &cameraComponent.Camera;
					cameraTransform = &transformComponent.Transform;
					break;
				}
			}
		}

		// Now we should have the active camera to render with
		if (activeCamera != nullptr)
		{
			Renderer2D::BeginScene(*activeCamera, *cameraTransform);

			auto entityView = m_Registry.view<TransformComponent,SpriteRendererComponent>();
			for (auto entity : entityView)
			{
				auto& [transformComponent, spriteComponent] = entityView.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transformComponent.Transform, spriteComponent.Colour);
			}

			Renderer2D::EndScene();
		}
	}
}
