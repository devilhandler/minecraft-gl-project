#include "mcpch.h"
#include "Application.h"

#include "Minecraft/Events/ApplicationEvent.h"
#include "Minecraft/Log.h"

namespace Minecraft
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			MC_TRACE(e);
		}
		while (true);
	}
}