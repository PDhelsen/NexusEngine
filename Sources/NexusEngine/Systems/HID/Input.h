#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Input
	{
		enum class State
		{
			Up,
			Pressed,
			Down,
			Released,
		};

		NEXUS_ENUM_TO_STRING_DEFINITION(NEXUS_ENGINE_API, State);

		enum class Axis
		{
			MouseX,
			MouseY,
			ScrollX,
			ScrollY,

			COUNT
		};

		enum class Button
		{
			Invalid,

			CapsLock,
			ScrollLock,
			NumLock,

			LeftShift,
			LeftControl,
			LeftAlt,
			RightShift,
			RightControl,
			RightAlt,

			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,

			Alpha0,
			Alpha1,
			Alpha2,
			Alpha3,
			Alpha4,
			Alpha5,
			Alpha6,
			Alpha7,
			Alpha8,
			Alpha9,

			Num0,
			Num1,
			Num2,
			Num3,
			Num4,
			Num5,
			Num6,
			Num7,
			Num8,
			Num9,
			NumDecimal,
			NumDivide,
			NumMultiply,
			NumSubtract,
			NumAdd,
			NumEnter,
			NumEqual,

			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,

			Space,
			Apostrophe,
			Comma,
			Period,
			Semicolon,
			Slash,
			Backslash,
			Minus,
			Equal,

			LeftBracket,
			RightBracket,
			Escape,
			Enter,
			Tab,
			Backspace,
			Insert,
			Delete,
			Right,
			Left,
			Down,
			Up,
			PageUp,
			PageDown,
			Home,
			End,
			PrintScreen,
			Pause,
			Menu,

			MouseLeft,
			MouseRight,
			MouseMiddle,
			Mouse1,
			Mouse2,
			Mouse3,
			Mouse4,
			Mouse5,

			COUNT
		};
	}
}
