#pragma once

#include <Minecraft.h>

class Sandbox2D : public Minecraft::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Minecraft::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Minecraft::Event& e) override;
private:
	Minecraft::Ref<Minecraft::VertexArray> m_VertexArray;
	Minecraft::Ref<Minecraft::Shader> m_Shader;

	Minecraft::Ref<Minecraft::Texture2D> m_Texture;

	Minecraft::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor{ 0.8f, 0.2f, 0.3f };
};