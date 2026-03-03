#include "mcpch.h"
#include "Minecraft/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Minecraft
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		MC_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		MC_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		MC_PROFILE_FUNCTION();

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), m_Position) 
			*
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1))
		;

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // DON'T SWITCH IT UP! PUTTING IT IN DIFFERENT ORDER CAUSES DIFFERENT CALCULATION, you know the matrix rules right.
		
	}
}