#pragma once

#include "Minecraft/Layer.h"

#include "Minecraft/Events/ApplicationEvent.h"
#include "Minecraft/Events/KeyEvent.h"
#include "Minecraft/Events/MouseEvent.h"

namespace Minecraft
{

	class MC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowsResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
	};
}