#pragma once

#include <stdio.h>
#ifdef MC_PLATFORM_WINDOWS

extern Minecraft::Application* Minecraft::CreateApplication();

int main(int argc, char** argv)
{
	Minecraft::Log::Init();
	MC_CORE_WARN("Initialized Log!");
	int a = 5;
	MC_INFO("Hello! Var={}", a);

	auto app{ Minecraft::CreateApplication() };
	app->Run();
	delete app;
}

#endif