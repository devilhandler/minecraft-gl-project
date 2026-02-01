#pragma once

#include "Core.h"

#include "Window.h"
#include "Minecraft/LayerStack.h"
#include "Minecraft/Events/Event.h"
#include "Minecraft/Events/ApplicationEvent.h"

namespace Minecraft
{
	class MC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in the Client
	Application* CreateApplication();
}