#pragma once

#include <glm/glm.hpp>

namespace Minecraft
{
	class Camera
	{
	public:
		Camera(uint32_t width, uint32_t height, const glm::vec3& cameraPosition);
		~Camera();

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetOrientation(const glm::vec3& orientation) { m_Orientation = orientation; }
		const glm::vec3& GetOrientation() const { return m_Orientation; }

		const glm::vec3& GetUpVector() const { return m_UpVector; }

		void SetCameraMatrix(const glm::mat4& cameraMatrix) { m_CameraMatrix = cameraMatrix; }
		const glm::mat4& GetCameraMatrix() const { return m_CameraMatrix; }
	private:
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Orientation{ 0.0f, 0.0f, -1.0f };
		glm::vec3 m_UpVector{ 0.0f, 1.0f, 0.0f };
		glm::mat4 m_CameraMatrix{ glm::mat4(1.0f) };

		bool m_FirstClick{ true };

		uint32_t m_Width{ 0 };
		uint32_t m_Height{ 0 };

		float m_Speed{ 0.005f };
		float m_Sensivity{ 10.0f };
	};
}