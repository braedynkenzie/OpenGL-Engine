#include "EnginePCH.h"
#include "SceneCamera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Engine {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void Engine::SceneCamera::SetOrthographic(float size, float nearClipBound, float farClipBound)
	{
		m_OrthoSize = size;
		m_OrthoNear = nearClipBound;
		m_OrthoFar  = farClipBound;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoRightBound = m_AspectRatio * m_OrthoSize * 0.5f;
		float orthoLeftBound = -orthoRightBound;
		float orthoTopBound = m_OrthoSize * 0.5f;
		float orthoBottomBound = -orthoTopBound;
		m_ProjectionMatrix = glm::ortho(orthoLeftBound, orthoRightBound, orthoTopBound, orthoBottomBound, m_OrthoNear, m_OrthoFar);
	}


}