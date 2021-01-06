#pragma once

#include <glm\glm.hpp>

//#include "Engine\Renderer\Camera.h"
#include "Engine\Scene\SceneCamera.h"
#include "Engine\Scene\ScriptableEntity.h"

namespace Engine {

	struct TransformComponent
	{
		glm::mat4 Transform  = glm::mat4(1.0f);

		// Constructors
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& modelMatrix)
			:Transform(modelMatrix) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct TagComponent
	{
		std::string Tag;

		// Constructors
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}


	};

	struct SpriteRendererComponent
	{
		glm::vec4 Colour = glm::vec4(1.0f);

		// Constructors
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& colour)
			:Colour(colour) {}
	};

	struct CameraComponent
	{
		Engine::SceneCamera Camera;
		bool IsPrimaryCamera = true; // TODO will likely move this to be handled by the scene class
		bool HasFixedAspectRatio = false; 

		// Constructors
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiationFunction;
		std::function<void()> DestructorFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		// Bind function allows binding to the ScriptableEntity's versions of the above functions
		template<typename T>
		void Bind()
		{
			InstantiationFunction = [&]() { Instance = new T(); };
			DestructorFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep deltaTime) { ((T*)instance)->OnUpdate(deltaTime); };
		}
	};

}
