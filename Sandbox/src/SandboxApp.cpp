#include <Minecraft.h>

class ExampleLayer : public Minecraft::Layer
{
public:
	ExampleLayer()
		: Layer("Example") { }


	void OnUpdate() override
	{
		if (Minecraft::Input::IsKeyPressed(MC_KEY_TAB))
			MC_INFO("Tab key is pressed (poll)!");
	}

	void OnEvent(Minecraft::Event& event) override
	{
		// MC_TRACE("{0}", event);
		if (event.GetEventType() == Minecraft::EventType::KeyPressed)
		{
			Minecraft::KeyPressedEvent& e = (Minecraft::KeyPressedEvent&)event;
			if (e.GetKeycode() == MC_KEY_TAB)
				MC_TRACE("Tab key is pressed (event)!");
			MC_TRACE("{0}", (char)e.GetKeycode());
		}
	}
};

class Sandbox : public Minecraft::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Minecraft::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Minecraft::Application* Minecraft::CreateApplication()
{
	return new Sandbox();
}