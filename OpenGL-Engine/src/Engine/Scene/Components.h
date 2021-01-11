#pragma once

#include <glm\glm.hpp>

#include "Engine\Scene\SceneCamera.h"
#include "Engine\Scene\ScriptableEntity.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Engine {

	struct TransformComponent
	{
		//glm::mat4 Transform  = glm::mat4(1.0f);
		glm::vec3 Translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Rotation	  = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Scale		  = glm::vec3(1.0f, 1.0f, 1.0f);

		// Constructors
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3 & translation)
			: Translation(translation) {}
		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation)
			: Translation(translation), Rotation(rotation) {}
		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: Translation(translation), Rotation(rotation), Scale(scale) {}

		glm::mat4 GetTransform()
		{
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::translate(transform, Translation);
			transform = glm::rotate(transform, Rotation.x, { 1, 0, 0 });
			transform = glm::rotate(transform, Rotation.y, { 0, 1, 0 });
			transform = glm::rotate(transform, Rotation.z, { 0, 0, 1 });
			transform = glm::scale(transform, Scale);
			return transform;
		}
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
		bool IsPrimaryCamera = false; 
		bool HasFixedAspectRatio = false; 

		// Constructors
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		// Bind function allows binding to the ScriptableEntity's versions of the above functions
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent) 
			{ 
				delete nativeScriptComponent->Instance; 
				nativeScriptComponent->Instance = nullptr; 
			};
		}
	};

}
