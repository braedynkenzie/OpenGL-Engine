#pragma once

#include <glm/glm.hpp>

namespace Engine {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position);

		inline float GetZRotation() const { return m_ZRotation; }
		void SetZRotation(float zRotation);

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetProjectionMatrix(float left, float right, float bottom, float top);

	private:
		void RecalculateViewMatrix();

	private:
		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		float m_ZRotation = 0.0f;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		// Optional -- caching VP matrix
		glm::mat4 m_ViewProjectionMatrix;

	};
}

