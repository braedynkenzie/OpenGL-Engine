#pragma once

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	// Essentially a wrapper class for camera to facilitate camera movement without bogging down camera objects
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotationEnabled = false);
		OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotationEnabled = false);

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& event);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel);

		const OrthographicCameraBounds& GetBounds() const { return m_CameraBounds; }

		void Resize(float width, float height);

	private:
		bool OnMouseScrollEvent(MouseScrollEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		float m_MinZoomLevel = 0.1f;
		float m_MaxZoomLevel = 100.0f;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraTranslationSpeed = 4.2f;

		bool m_RotationEnabled;
		float m_Rotation = 0.0f; // counter-clockwise degrees
		float m_RotationSpeed = 180.0f;

		OrthographicCameraBounds m_CameraBounds;
		OrthographicCamera m_Camera;

	};
}