#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Input
	{
		enum class Modifier : uint8
		{
			None = 0,
			Ignore = 1 << 0,

			Ctrl = 1 << 1,
			Shift = 1 << 2,
			Alt = 1 << 3,
		};

		enum class State
		{
			Up,
			Pressed,
			Down,
			Released,

			COUNT
		};

		enum class Mode
		{
			Button,
			Axis,
			Mouse,

			COUNT
		};

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

NEXUS_FLAG(NxEn::Input::Modifier, uint8);

NEXUS_FLAG_STRING(NxEn::Input::Modifier	, 4										, "Ignore", "Ctrl", "Shift", "Alt");
NEXUS_ENUM_STRING(NxEn::Input::State	, (uint64)NxEn::Input::State::COUNT		, "Up", "Pressed", "Down", "Released");
NEXUS_ENUM_STRING(NxEn::Input::Mode		, (uint64)NxEn::Input::Mode::COUNT		, "Button", "Axis", "Mouse");
NEXUS_ENUM_STRING(NxEn::Input::Axis		, (uint64)NxEn::Input::Axis::COUNT		, "MouseX", "MouseY", "ScrollX", "ScrollY");
NEXUS_ENUM_STRING(NxEn::Input::Button	, (uint64)NxEn::Input::Button::COUNT	,
	"Invalid",
	"CapsLock",
	"ScrollLock",
	"NumLock",
	"LeftShift",
	"LeftControl",
	"LeftAlt",
	"RightShift",
	"RightControl",
	"RightAlt",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Alpha0",
	"Alpha1",
	"Alpha2",
	"Alpha3",
	"Alpha4",
	"Alpha5",
	"Alpha6",
	"Alpha7",
	"Alpha8",
	"Alpha9",
	"Num0",
	"Num1",
	"Num2",
	"Num3",
	"Num4",
	"Num5",
	"Num6",
	"Num7",
	"Num8",
	"Num9",
	"NumDecimal",
	"NumDivide",
	"NumMultiply",
	"NumSubtract",
	"NumAdd",
	"NumEnter",
	"NumEqual",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"Space",
	"Apostrophe",
	"Comma",
	"Period",
	"Semicolon",
	"Slash",
	"Backslash",
	"Minus",
	"Equal",
	"LeftBracket",
	"RightBracket",
	"Escape",
	"Enter",
	"Tab",
	"Backspace",
	"Insert",
	"Delete",
	"Right",
	"Left",
	"Down",
	"Up",
	"PageUp",
	"PageDown",
	"Home",
	"End",
	"PrintScreen",
	"Pause",
	"Menu",
	"MouseLeft",
	"MouseRight",
	"MouseMiddle",
	"Mouse1",
	"Mouse2",
	"Mouse3",
	"Mouse4",
	"Mouse5",
	);

namespace NxEn
{
	namespace Input
	{
		union Binding
		{
			struct BindingButton
			{
				NEXUS_ENGINE_API BindingButton(Button ButtonInput, State ButtonState);

				Button ButtonInput;
				State ButtonState;
			};

			NEXUS_ENGINE_API Binding(Button ButtonInput, State ButtonState);
			NEXUS_ENGINE_API Binding(Axis InputAxis);
			NEXUS_ENGINE_API Binding(NxFr::Rectangle InputMouse);
			NEXUS_ENGINE_API ~Binding();

			NEXUS_ENGINE_API Binding& operator=(BindingButton Other);
			NEXUS_ENGINE_API Binding& operator=(Axis Other);
			NEXUS_ENGINE_API Binding& operator=(NxFr::Rectangle Other);

			BindingButton InputButton;
			Axis InputAxis;
			NxFr::Rectangle InputMouse;
		};

		struct Trigger
		{
		public:
			NEXUS_ENGINE_API Trigger(Button ButtonInput, State ButtonTarget, Modifier Modifiers = Modifier::Ignore);
			NEXUS_ENGINE_API Trigger(Axis AxisInput, Modifier Modifiers = Modifier::Ignore);
			NEXUS_ENGINE_API Trigger(NxFr::Rectangle MouseInput, Modifier Modifiers = Modifier::Ignore);
			NEXUS_ENGINE_API Trigger(const Trigger& Other);
			NEXUS_ENGINE_API ~Trigger();

			NEXUS_ENGINE_API Trigger& operator=(const Trigger& Other);

			NEXUS_ENGINE_API Mode GetMode() const;
			NEXUS_ENGINE_API Modifier GetModifiers() const;

			NEXUS_ENGINE_API Button GetInputButton() const;
			NEXUS_ENGINE_API State GetInputButtonState() const;
			NEXUS_ENGINE_API Axis GetInputAxis() const;
			NEXUS_ENGINE_API NxFr::Rectangle GetInputMouse() const;


		private:
			Mode Mode;
			Modifier Modifiers;
			Binding Input;
		};

		struct Action
		{
		public:
			NEXUS_ENGINE_API Action(const Trigger& Input, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(Button ButtonInput, State ButtonTarget, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(Axis AxisInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(NxFr::Rectangle MouseInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(const Action& Other);
			NEXUS_ENGINE_API ~Action();

			NEXUS_ENGINE_API Action& operator=(const Action& Other);

			NEXUS_ENGINE_API const Trigger& GetTrigger() const;
			NEXUS_ENGINE_API const NxFr::Delegate<void()>& GetCallback() const;

		private:
			Trigger Input;
			NxFr::Delegate<void()> Callback;
		};

		struct Schema
		{
		public:
			NEXUS_ENGINE_API Schema();
			NEXUS_ENGINE_API ~Schema();

			NEXUS_ENGINE_API NxFr::Dictionary<NxFr::StringId, Action>& GetMapping();

		private:
			NxFr::Dictionary<NxFr::StringId, Action> Actions;
		};
	}
}
