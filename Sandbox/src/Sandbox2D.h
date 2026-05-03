#pragma once

#include <RogueEngine.h>

class Sandbox2D : public Rogue::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Rogue::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Rogue::Event& e) override;
private:
	Rogue::Ref<Rogue::VertexArray> m_VertexArray;
	Rogue::Ref<Rogue::Shader> m_Shader;

	Rogue::OrthographicCameraController m_CameraController;

	// Temp
	Rogue::Ref<Rogue::Texture2D> m_Texture;
	Rogue::Ref<Rogue::Texture2D> m_SpriteSheet;
	Rogue::Ref<Rogue::SubTexture2D> m_Stairs, m_Barrel, m_Tree;
	Rogue::Ref<Rogue::Framebuffer> m_Framebuffer;

	glm::vec4 m_SquareColor{ 0.8f, 0.2f, 0.3f, 1.0f };

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Rogue::Ref<Rogue::SubTexture2D>> s_TextureMap;
};