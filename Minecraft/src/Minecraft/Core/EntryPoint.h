#pragma once
#include "Minecraft/Core/Core.h"

#ifdef MC_PLATFORM_WINDOWS

extern Minecraft::Application* Minecraft::CreateApplication();

int main(int argc, char** argv)
{
	Minecraft::Log::Init();
	
	MC_PROFILE_BEGIN_SESSION("Startup", "MinecraftProfile.Startup.json");
	auto app{ Minecraft::CreateApplication() };
	MC_PROFILE_END_SESSION();

	MC_PROFILE_BEGIN_SESSION("Runtime", "MinecraftProfile.Runtime.json");
	app->Run();
	MC_PROFILE_END_SESSION();

	MC_PROFILE_BEGIN_SESSION("Shutdown", "MinecraftProfile.Shutdown.json");
	delete app;
	MC_PROFILE_END_SESSION();
}

#endif