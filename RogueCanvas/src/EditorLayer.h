#pragma once

#include <RogueEngine.h>

namespace Rogue
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;

		OrthographicCameraController m_CameraController;

		// Temp
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_Stairs, m_Barrel, m_Tree;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec4 m_SquareColor{ 0.8f, 0.2f, 0.3f, 1.0f };
		glm::vec2 m_ViewportSize{ 0.0f, 0.0f };

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
	};
}