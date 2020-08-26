#include "EnginePCH.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ViewMatrix(glm::mat4(1.0f)), // default camera position is at origin
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetZRotation(float zRotation)
	{
		m_ZRotation = zRotation; 
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjectionMatrix(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// Create the View Matrix by inverting the camera position/rotation transformations 
		glm::mat4 transformationMatrix = glm::mat4(1.0f);
		// Translation
		transformationMatrix = glm::translate(transformationMatrix, m_Position);
		// Rotation (z axis only, since this is an orthographic camera)
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_ZRotation), glm::vec3(0.0f, 0.0f, 1.0f));
		// Now take the inverse to get the View Matrix
		m_ViewMatrix = glm::inverse(transformationMatrix);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
