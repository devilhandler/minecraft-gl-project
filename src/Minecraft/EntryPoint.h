#pragma once

#include <stdio.h>
#ifdef MC_PLATFORM_WINDOWS

extern Minecraft::Application* Minecraft::CreateApplication();

int main(int argc, char** argv)
{
	auto app{ Minecraft::CreateApplication() };
	app->Run();
	delete app;
}

#endif