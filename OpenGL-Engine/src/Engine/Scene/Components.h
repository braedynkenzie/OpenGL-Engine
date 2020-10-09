#pragma once

#include <glm\glm.hpp>

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

	struct SpriteRendererComponent
	{
		glm::vec4 Colour = glm::vec4(1.0f);

		// Constructors
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& colour)
			:Colour(colour) {}
	};

}
