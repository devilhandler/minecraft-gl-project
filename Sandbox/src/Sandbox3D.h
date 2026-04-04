#pragma once

#include <Minecraft.h>

class Sandbox3D : public Minecraft::Layer 
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Minecraft::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Minecraft::Event& e) override;
private:
	Minecraft::Ref<Minecraft::VertexArray> m_VertexArray;
	Minecraft::Ref<Minecraft::Shader> m_Shader;

	Minecraft::Ref<Minecraft::Texture2D> m_Texture;

	// Minecraft::Ref<Minecraft::CameraController> m_Camera;
};