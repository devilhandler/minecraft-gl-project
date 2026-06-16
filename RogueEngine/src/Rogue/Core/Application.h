#pragma once

#include "Rogue/Core/Core.h"

#include "Rogue/Core/Window.h"
#include "Rogue/Core/LayerStack.h"
#include "Rogue/Events/Event.h"
#include "Rogue/Events/ApplicationEvent.h"

#include "Rogue/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Rogue
{
	class Application
	{
	public:
		Application(const std::string& name = "Rogue App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		inline Scope<Window>& GetWindow() { return m_Window; }
		
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
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