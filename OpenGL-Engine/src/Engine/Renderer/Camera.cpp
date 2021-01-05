#include "EnginePCH.h"
#include "Camera.h"

namespace Engine {

	const glm::mat4& Engine::Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

}
