#pragma once

#include <glm\glm.hpp>

namespace Engine {

	class Camera
	{
	public:
		Camera(const glm::mat4 projMatrix)
			: m_ProjectionMatrix(projMatrix)
		{}

		const glm::mat4& GetProjectionMatrix() const;

		// TODO: Add functions such as "SetPerspective" and "SetOrthographic"

	private:
		// Note this camera class only requires the projection since the camera position and orientation is a part of the camera entity's transformation matrix
		glm::mat4 m_ProjectionMatrix;
	};

}

