#pragma once

#include <glm\glm.hpp>

#include "Engine\Renderer\Camera.h"

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
		Engine::Camera Camera;
		bool IsPrimaryCamera = true; // TODO will likely move this to be handled by the scene class

		// Constructors
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4 projectionMatrix)
			: Camera(Engine::Camera(projectionMatrix)) {}
	};

}
