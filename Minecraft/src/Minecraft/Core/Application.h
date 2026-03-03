#pragma once

#include "Core.h"

#include "Window.h"
#include "Minecraft/Core/LayerStack.h"
#include "Minecraft/Events/Event.h"
#include "Minecraft/Events/ApplicationEvent.h"

#include "Minecraft/ImGui/ImGuiLayer.h"

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

		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Window* m_Window;
		ImGuiLayer* m_ImGuiLayer{ nullptr };
		bool m_Running{ true };
		bool m_Minimized{ false };
		LayerStack m_LayerStack;
		float m_LastFrameTime{ 0.0f };
	private:
		static Application* s_Instance;
	};

	// To be defined in the Client
	Application* CreateApplication();
}