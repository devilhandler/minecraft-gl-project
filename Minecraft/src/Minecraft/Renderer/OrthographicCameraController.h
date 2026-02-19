#pragma once

#include "Minecraft/Core/Timestep.h"
#include "Minecraft/Renderer/OrthographicCamera.h"

#include "Minecraft/Events/ApplicationEvent.h"
#include "Minecraft/Events/MouseEvent.h"

namespace Minecraft
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio{ 0.0f };
		float m_ZoomLevel{ 1.0f };
		OrthographicCamera m_Camera;

		bool m_Rotation{ false };
		glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
		float m_CameraRotation{ 0.0f };
		float m_CameraTranslationSpeed{ 5.0f }, m_CameraRotationSpeed{ 180.0f };
	};
}