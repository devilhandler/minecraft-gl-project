#include "mcpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/imguiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Minecraft/Application.h"

namespace Minecraft
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float dT = (float) glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (dT - m_Time) : 1.0f / 60.0f;
		m_Time = dT;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& e)
	{

	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{

	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{

	}
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{

	}
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{

	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{

	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{

	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{

	}
	bool ImGuiLayer::OnWindowsResizedEvent(WindowResizeEvent& e)
	{

	}
}