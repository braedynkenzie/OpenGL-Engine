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
		// Update scripts
		{
			// Iterate through all entities with a native script component
			auto scriptComponentsView = m_Registry.view<NativeScriptComponent>();
			for (auto entity : scriptComponentsView)
			{
				auto& nativeScriptComponent = scriptComponentsView.get<NativeScriptComponent>(entity);

				// If the script component is not yet instantiated, do so
				if (!nativeScriptComponent.Instance)
				{
					nativeScriptComponent.InstantiationFunction();
					// Now that the script component is instanced, set its Entity and call the OnCreate function if it exists
					nativeScriptComponent.Instance->m_Entity = Entity{ entity, this }; // TODO figure out why m_Entity can be null
					if(nativeScriptComponent.OnCreateFunction)
						nativeScriptComponent.OnCreateFunction(nativeScriptComponent.Instance);
				}

				if (nativeScriptComponent.OnUpdateFunction)
					nativeScriptComponent.OnUpdateFunction(nativeScriptComponent.Instance, deltaTime);
			}
		}

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

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		
		// Resize all cameras in the scene that do not have a fixed aspect ratio
		auto entityView = m_Registry.view<CameraComponent>();
		for (auto entity : entityView)
		{
			CameraComponent& cameraComponent = entityView.get<CameraComponent>(entity);
			if (!cameraComponent.HasFixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width,height);
			}
		}

	}
}
