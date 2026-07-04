#pragma once

#include "Rogue/Core/Layer.h"

#include "Rogue/Events/ApplicationEvent.h"
#include "Rogue/Events/KeyEvent.h"
#include "Rogue/Events/MouseEvent.h"

namespace Rogue
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents{ false };
		float m_Time = 0.0f;
	};
}