#pragma once

namespace Rogue
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

// From glfw3.h //
/*! @defgroup buttons Mouse buttons
 *  @brief Mouse button IDs.
 *
 *  See [mouse button input](@ref input_mouse_button) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define RE_MOUSE_BUTTON_1         0
#define RE_MOUSE_BUTTON_2         1
#define RE_MOUSE_BUTTON_3         2
#define RE_MOUSE_BUTTON_4         3
#define RE_MOUSE_BUTTON_5         4
#define RE_MOUSE_BUTTON_6         5
#define RE_MOUSE_BUTTON_7         6
#define RE_MOUSE_BUTTON_8         7
#define RE_MOUSE_BUTTON_LAST      RE_MOUSE_BUTTON_8
#define RE_MOUSE_BUTTON_LEFT      RE_MOUSE_BUTTON_1
#define RE_MOUSE_BUTTON_RIGHT     RE_MOUSE_BUTTON_2
#define RE_MOUSE_BUTTON_MIDDLE    RE_MOUSE_BUTTON_3
 /*! @} */