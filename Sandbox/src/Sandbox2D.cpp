#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imguidock/imgui.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
	
}

void Sandbox2D::OnAttach()
{
	m_Texture = Minecraft::Texture2D::Create("assets/textures/grass.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Minecraft::Timestep ts)
{
	MC_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Minecraft::Renderer2D::ResetStats();
	{
		MC_PROFILE_SCOPE("Renderer Prep");
		Minecraft::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Minecraft::RenderCommand::Clear();
	}

	{
		MC_PROFILE_SCOPE("Renderer Draw");

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Minecraft::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Minecraft::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Minecraft::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Minecraft::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Minecraft::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, 10.0f);
		Minecraft::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_Texture, 20.0f);
		Minecraft::Renderer2D::EndScene();

		Minecraft::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Minecraft::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		/* {
			for (unsigned int y{ 0 }; y < 20; ++y)
			{
				for (unsigned int x{ 0 }; x < 20; ++x)
				{
					glm::vec2 pos(x * 0.11f, y * 0.11f);
					if (x % 2 == 0)
						Minecraft::Renderer2D::DrawRotatedQuad(pos, { 0.1f, 0.1f }, glm::radians(45.0f), m_SquareColor);
					else
						Minecraft::Renderer2D::DrawQuad(pos, { 0.1f, 0.1f }, m_SquareColor - glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
				}
			}

			Minecraft::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
			//Minecraft::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, {0.2f, 0.3f, 0.8f, 1.0f});
		} */
		Minecraft::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	MC_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Minecraft::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Minecraft::Event& e)
{
	m_CameraController.OnEvent(e);
}