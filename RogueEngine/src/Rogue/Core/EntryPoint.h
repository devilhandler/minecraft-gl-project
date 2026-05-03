#pragma once
#include "Rogue/Core/Core.h"

#ifdef RE_PLATFORM_WINDOWS

extern Rogue::Application* Rogue::CreateApplication();

int main(int argc, char** argv)
{
	Rogue::Log::Init();
	
	RE_PROFILE_BEGIN_SESSION("Startup", "RogueEngineProfile.Startup.json");
	auto app{ Rogue::CreateApplication() };
	RE_PROFILE_END_SESSION();

	// TODO: Fix memory leak, literally prints so much string to the json file.
	// RE_PROFILE_BEGIN_SESSION("Runtime", "RogueEngineProfile.Runtime.json");
	app->Run();
	// RE_PROFILE_END_SESSION();

	RE_PROFILE_BEGIN_SESSION("Shutdown", "RogueEngineProfile.Shutdown.json");
	delete app;
	RE_PROFILE_END_SESSION();
}

#endif