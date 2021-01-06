#include "EnginePCH.h"
#include "OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace Engine {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationEnabled)
		: m_AspectRatio(aspectRatio), 
		m_RotationEnabled(rotationEnabled),
		m_CameraBounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(OrthographicCamera(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top))
	{
	}

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotationEnabled)
		: m_AspectRatio(aspectRatio),
		m_ZoomLevel(zoomLevel),
		m_RotationEnabled(rotationEnabled),
		m_CameraBounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(OrthographicCamera(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top))
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		ENGINE_PROFILE_FUNCTION();

		// Change camera movement speed depending on zoom level
		m_CameraTranslationSpeed = m_ZoomLevel;

		// WASD-bound camera movement
		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y += cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y -= sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		if (Input::IsKeyPressed(KeyCode::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y -= cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		if (Input::IsKeyPressed(KeyCode::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
			m_CameraPosition.y += sin(glm::radians(m_Rotation)) * m_CameraTranslationSpeed * (float)deltaTime;
		}
		// Apply camera position changes
		m_Camera.SetPosition(m_CameraPosition);

		if (m_RotationEnabled)
		{
			// E and Q keys to rotate camera
			if (Input::IsKeyPressed(KeyCode::E))
				m_Rotation -= m_RotationSpeed * (float)deltaTime;
			if (Input::IsKeyPressed(KeyCode::Q))
				m_Rotation += m_RotationSpeed * (float)deltaTime;
			// Bind rotation between -180 to 180 degrees
			if (m_Rotation > 180.0f)
				m_Rotation -= 360.0f;
			else if (m_Rotation <= -180.0f)
				m_Rotation += 360.0f;
			// Apply camera rotation changes
			m_Camera.SetZRotation(m_Rotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		ENGINE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>(ENGINE_BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrollEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResizeEvent));
	}

	void OrthographicCameraController::SetZoomLevel(float zoomLevel)
	{
		m_ZoomLevel = zoomLevel;
		m_Camera.RecalculateViewMatrix();
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_CameraBounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjectionMatrix(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrollEvent(MouseScrollEvent& event)
	{
		ENGINE_PROFILE_FUNCTION();

		m_ZoomLevel -= event.GetYOffset() * 0.05f;
		// Clamp zoom level
		m_ZoomLevel = std::max(m_ZoomLevel, m_MinZoomLevel);
		m_ZoomLevel = std::min(m_ZoomLevel, m_MaxZoomLevel);
		m_CameraBounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjectionMatrix(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ENGINE_PROFILE_FUNCTION();
		Resize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}

}