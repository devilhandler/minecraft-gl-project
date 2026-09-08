#pragma once

#include "Rogue/Core/Core.h"
#include "Rogue/Core/KeyCodes.h"
#include "Rogue/Core/MouseCodes.h"

namespace Rogue
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}