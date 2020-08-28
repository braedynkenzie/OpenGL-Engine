#include "EnginePCH.h"
#include "OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace Engine {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationEnabled)
		: m_AspectRatio(aspectRatio), 
		m_RotationEnabled(rotationEnabled),
		m_Camera(OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel))
	{
	}

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotationEnabled)
		: m_AspectRatio(aspectRatio),
		m_ZoomLevel(zoomLevel),
		m_RotationEnabled(rotationEnabled),
		m_Camera(OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel))
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		// WASD-bound camera movement
		if (Input::IsKeyPressed(ENGINE_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y += cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		if (Input::IsKeyPressed(ENGINE_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y -= sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		if (Input::IsKeyPressed(ENGINE_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y -= cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		if (Input::IsKeyPressed(ENGINE_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y += sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		// Apply camera position changes
		m_Camera.SetPosition(m_CameraPosition);

		if (m_RotationEnabled)
		{
			// E and Q keys to rotate camera
			if (Input::IsKeyPressed(ENGINE_KEY_E))
				m_Rotation -= m_RotationSpeed * (float)deltaTime;
			if (Input::IsKeyPressed(ENGINE_KEY_Q))
				m_Rotation += m_RotationSpeed * (float)deltaTime;
			// Bind rotation between -180 to 180 degrees
			if (m_Rotation > 180.0f)
				m_Rotation -= 360.0f;
			else if (m_Rotation <= -180.0f)
				m_Rotation += 360.0f;
			// Apply camera rotation changes
			m_Camera.SetZRotation(m_Rotation);
		}

		// Change camera movement speed depending on zoom level
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>(ENGINE_BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrollEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResizeEvent));
	}

	bool OrthographicCameraController::OnMouseScrollEvent(MouseScrollEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.05f;
		// Clamp zoom level
		m_ZoomLevel = std::max(m_ZoomLevel, 0.4f);
		m_ZoomLevel = std::min(m_ZoomLevel, 10.0f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}