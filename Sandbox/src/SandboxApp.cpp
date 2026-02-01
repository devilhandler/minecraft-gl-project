#include <Minecraft.h>

class ExampleLayer : public Minecraft::Layer
{
public:
	ExampleLayer()
		: Layer("Example") { }


	void OnUpdate() override
	{
		MC_INFO("ExampleLayer::Update");
	}

	void OnEvent(Minecraft::Event& event) override
	{
		MC_TRACE("{0}", event);
	}
};

class Sandbox : public Minecraft::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Minecraft::Application* Minecraft::CreateApplication()
{
	return new Sandbox();
}