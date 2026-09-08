#include "repch.h"
#include "Rogue/Renderer/OrthographicCameraController.h"

#include "Rogue/Core/Input.h"
#include "Rogue/Core/KeyCodes.h"

namespace Rogue
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{
		
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		RE_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(KeyCode::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(KeyCode::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(KeyCode::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(KeyCode::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(KeyCode::R))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		RE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		RE_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		RE_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}