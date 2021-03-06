#include "EnginePCH.h"

#include "Entity.h"
#include "Scene.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		// Create the entity from a handle
		entt::entity entityHandle = m_Registry.create();
		Entity entity = Entity(entityHandle, this);
		// Add a TransformComponent to all entities
		entity.AddComponent<TransformComponent>();
		// Add a TagComponent to all entities
		auto& tagComponent = entity.AddComponent<TagComponent>(name);
		tagComponent.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
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

				// TODO: move the following stuff to Scene::OnScenePlay or whatever it ends up being called
				// If the script component is not yet instantiated, do so
				if (!nativeScriptComponent.Instance)
				{
					nativeScriptComponent.Instance = nativeScriptComponent.InstantiateScript();
					// Now that the script component is instanced, set its Entity and call the OnCreate function if it exists
					nativeScriptComponent.Instance->m_Entity = Entity{ entity, this }; 
					nativeScriptComponent.Instance->OnCreate();
				}

				nativeScriptComponent.Instance->OnUpdate(deltaTime);
			}
		}

		// Render
		Camera* activeCamera = nullptr;
		glm::mat4 cameraTransform;

		// First, we find the main camera for the scene from one of the entities
		// Get a group/"view" of all entities will a camera component and transform component
		{
			auto entityView = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : entityView)
			{
				// Get the camera's transform (ie its uninverted view matrix) and the camera component (containing the projection matrix)
				auto [transformComponent, cameraComponent] = entityView.get<TransformComponent, CameraComponent>(entity);

				if (cameraComponent.IsPrimaryCamera)
				{
					activeCamera = &cameraComponent.Camera;
					cameraTransform = transformComponent.GetTransform();
					break;
				}
			}
		}

		// Now we should have the active camera to render with
		if (activeCamera != nullptr)
		{
			Renderer2D::BeginScene(*activeCamera, cameraTransform);

			auto entityView = m_Registry.view<TransformComponent,SpriteRendererComponent>();
			for (auto entity : entityView)
			{
				auto [transformComponent, spriteComponent] = entityView.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteComponent.Colour);
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

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		// Never supposed to not have a specialization for a component type
		// (see below for specialized overloads)
		ENGINE_ASSERT(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& tagComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& transformComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& cameraComponent)
	{
		cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& cameraComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& nativeScriptComponent)
	{
	}
}
