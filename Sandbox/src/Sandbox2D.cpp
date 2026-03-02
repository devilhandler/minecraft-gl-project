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
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Minecraft::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Minecraft::RenderCommand::Clear();

	Minecraft::Renderer2D::BeginScene(m_CameraController.GetCamera());
	{

		for (unsigned int y{ 0 }; y < 20; ++y)
		{
			for (unsigned int x{ 0 }; x < 20; ++x)
			{
				glm::vec2 pos(x * 0.11f, y * 0.11f);
				if (x % 2 == 0)
					Minecraft::Renderer2D::DrawQuad(pos, { 0.1f, 0.1f }, glm::vec4(m_SquareColor, 1.0f));
				else
					Minecraft::Renderer2D::DrawQuad(pos, { 0.1f, 0.1f }, glm::vec4(glm::vec3(0.8f) - m_SquareColor, 1.0f));
			}
		}

		Minecraft::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture);
		//Minecraft::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, {0.2f, 0.3f, 0.8f, 1.0f});
	}
	Minecraft::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Minecraft::Event& e)
{
	m_CameraController.OnEvent(e);
}