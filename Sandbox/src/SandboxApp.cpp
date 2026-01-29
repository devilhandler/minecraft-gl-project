#include <Minecraft.h>

class Sandbox : public Minecraft::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{

	}
};

Minecraft::Application* Minecraft::CreateApplication()
{
	return new Sandbox();
}