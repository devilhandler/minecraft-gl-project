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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}