#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imguidock/imgui.h>


static const uint32_t s_MapWidth{ 24 };
static const char* s_MapTiles
{
	"WWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWDDDDDDDDDDWWWWWWWW"
	"WWWWDDDDDDDDDDDDDDWCWWWW"
	"WWWDDDDDDDDDDDDDDDDWWWWW"
	"WWDDDDDDDDDDDDDDDDDDWWWW"
	"WDDDDDWWWDDDDDDDDWWWDWWW"
	"WDDDDDWWWDDDDDDDDWWWDDWW"
	"WWDDDDDDDDDDDDDDDDDDDWWW"
	"WWWDDDDDDDDDDDDDDDDDWWWW"
	"WWWWDDDDDDDDDDDDDDDWWWWW"
	"WWWWWDDDDDDDDDDDDDWWWWWW"
	"WWWWWWWDDDDDDDDDWWWWWWWW"
	"WWWWWWWWWWDDDDWWWWWWWWWW"
};

struct ImGuiDockspaceArgs
{
	bool                IsFullscreen = true;
	bool                KeepWindowPadding = false; // Keep WindowPadding to help understand that DockSpace() is a widget inside the window.
	ImGuiDockNodeFlags  DockSpaceFlags = ImGuiDockNodeFlags_None;
};
static ImGuiDockspaceArgs s_DockspaceArgs{ true, false, ImGuiDockNodeFlags_PassthruCentralNode };

namespace Rogue
{

	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		m_Texture = Rogue::Texture2D::Create("assets/textures/grass.png");
		m_SpriteSheet = Rogue::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		s_TextureMap['D'] = Rogue::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
		s_TextureMap['W'] = Rogue::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });

		m_Stairs = Rogue::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 });
		m_Barrel = Rogue::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 1 }, { 128, 128 });
		m_Tree = Rogue::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

		m_CameraController.SetZoomLevel(5.0f);

		Rogue::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Rogue::Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Rogue::Timestep ts)
	{
		RE_PROFILE_FUNCTION();

		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Rogue::Renderer2D::ResetStats();
		{
			RE_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			Rogue::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Rogue::RenderCommand::Clear();
		}

#if 0
		{
			RE_PROFILE_SCOPE("Renderer Draw");

			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			Rogue::Renderer2D::BeginScene(m_CameraController.GetCamera());
			Rogue::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			Rogue::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Rogue::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
			Rogue::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, 10.0f);
			Rogue::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_Texture, 20.0f);
			Rogue::Renderer2D::EndScene();

			Rogue::Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
					Rogue::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}

			/* {
				for (unsigned int y{ 0 }; y < 20; ++y)
				{
					for (unsigned int x{ 0 }; x < 20; ++x)
					{
						glm::vec2 pos(x * 0.11f, y * 0.11f);
						if (x % 2 == 0)
							Rogue::Renderer2D::DrawRotatedQuad(pos, { 0.1f, 0.1f }, glm::radians(45.0f), m_SquareColor);
						else
							Rogue::Renderer2D::DrawQuad(pos, { 0.1f, 0.1f }, m_SquareColor - glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
					}
				}

				Rogue::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
				//Rogue::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, {0.2f, 0.3f, 0.8f, 1.0f});
			} */
			Rogue::Renderer2D::EndScene();
		}
#endif

		Rogue::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (uint32_t y{ 0 }; y < m_MapHeight; ++y)
		{
			for (uint32_t x{ 0 }; x < m_MapWidth; ++x)
			{
				char tileType{ s_MapTiles[x + y * m_MapWidth] };
				Rogue::Ref<Rogue::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
					texture = s_TextureMap[tileType];
				else
					texture = m_Barrel;

				Rogue::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
			}
		}

		Rogue::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Stairs);
		Rogue::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Barrel);
		Rogue::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f, 2.0f }, m_Tree);
		Rogue::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		RE_PROFILE_FUNCTION();

		// Note: switch this to true to enable dockspace
		static bool dockingEnabled{ true };
		if (dockingEnabled)
		{
			static bool dockspaceOpen{ true };

			ImGui::DockSpaceOverViewport(0, nullptr);
			ImGui::Begin("Dockspace", &dockspaceOpen, ImGuiWindowFlags_MenuBar);

			ImGui::SeparatorText("Options");
			s_DockspaceArgs.DockSpaceFlags &= ImGuiDockNodeFlags_PassthruCentralNode; // Allowed flags
			ImGui::CheckboxFlags("Flag: PassthruCentralNode", &s_DockspaceArgs.DockSpaceFlags, ImGuiDockNodeFlags_PassthruCentralNode);

			// Show demo options and help
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) Rogue::Application::Get().Close();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// Begin: ImGui Renderer2D Stats
			ImGui::Begin("Settings");

			auto stats = Rogue::Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			uint32_t textureID{ m_Framebuffer->GetColorAttachmentRendererID() };
			ImGui::Image(textureID, ImVec2{ 1280.0f, 720.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			// End: ImGui Renderer2D Stats

			ImGui::End();
		}
		else
		{
			// Begin: ImGui Renderer2D Stats
			ImGui::Begin("Settings");

			auto stats = Rogue::Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			ImGui::Image(m_Texture->GetRendererID(), ImVec2{ 256.0f, 256.0f });
			ImGui::End();
			// End: ImGui Renderer2D Stats
		}
	}

	void EditorLayer::OnEvent(Rogue::Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}