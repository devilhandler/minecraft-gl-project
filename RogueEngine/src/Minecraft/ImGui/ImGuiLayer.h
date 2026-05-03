#pragma once

#include "Minecraft/Core/Layer.h"

#include "Minecraft/Events/ApplicationEvent.h"
#include "Minecraft/Events/KeyEvent.h"
#include "Minecraft/Events/MouseEvent.h"

namespace Minecraft
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}