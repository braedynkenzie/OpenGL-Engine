#include "EnginePCH.h"
#include "Camera.h"

namespace Engine {

	const glm::mat4& Engine::Camera::GetProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

}
