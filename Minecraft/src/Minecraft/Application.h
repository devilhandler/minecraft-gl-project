#pragma once

#include "Core.h"

#include "Window.h"
#include "Minecraft/LayerStack.h"
#include "Minecraft/Events/Event.h"
#include "Minecraft/Events/ApplicationEvent.h"

#include "Minecraft/ImGui/ImGuiLayer.h"

// TEMPORARY
#include "Minecraft/Renderer/Shader.h"
#include "Minecraft/Renderer/Buffer.h"

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

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	// To be defined in the Client
	Application* CreateApplication();
}