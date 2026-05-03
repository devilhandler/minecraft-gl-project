#pragma once

#include <RogueEngine.h>

class Sandbox3D : public Rogue::Layer 
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Rogue::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Rogue::Event& e) override;
private:
	Rogue::Ref<Rogue::VertexArray> m_VertexArray;
	Rogue::Ref<Rogue::Shader> m_Shader;

	Rogue::Ref<Rogue::Texture2D> m_Texture;

	// Rogue::Ref<Rogue::CameraController> m_Camera;
};