#pragma once

#include "Minecraft/Layer.h"

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
		void OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		void OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		void OnMouseMovedEvent(MouseMovedEvent& e);
		void OnMouseScrolledEvent(MouseScrolledEvent& e);
		void OnKeyPressedEvent(KeyPressedEvent& e);
		void OnKeyReleasedEvent(KeyReleasedEvent& e);
		void OnKeyTypedEvent(KeyTypedEvent& e);
		void OnWindowsResizedEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
	};
}