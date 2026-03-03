#include "mcpch.h"
#include "Minecraft/Core/Application.h"

#include "Minecraft/Renderer/Renderer.h"
#include "Minecraft/Core/Timestep.h"

#include <GLFW/glfw3.h>

namespace Minecraft
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MC_PROFILE_FUNCTION();

		MC_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(MC_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MC_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		MC_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		MC_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		MC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(MC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(MC_BIND_EVENT_FN(Application::OnWindowResize));

		// MC_CORE_INFO("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		MC_PROFILE_FUNCTION();

		while (m_Running)
		{
			MC_PROFILE_SCOPE("RunLoop");

			float time{ (float) glfwGetTime() }; // Platform::GetTime()
			Timestep timestep{ time - m_LastFrameTime };
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					MC_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				// ImGui Render
				m_ImGuiLayer->Begin();
				{
					MC_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		};
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		MC_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}