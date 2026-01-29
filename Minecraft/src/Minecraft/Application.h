#pragma once

#include "Core.h"

namespace Minecraft
{
	class MC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in the Client
	Application* CreateApplication();
}