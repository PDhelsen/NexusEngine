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

NX_FLAG(NxEn::Input::Modifier, uint8);

NX_FLAG_STRING(NxEn::Input::Modifier	, 4										, "Ignore", "Ctrl", "Shift", "Alt");
NX_ENUM_STRING(NxEn::Input::State	, (uint64)NxEn::Input::State::COUNT		, "Up", "Pressed", "Down", "Released");
NX_ENUM_STRING(NxEn::Input::Mode		, (uint64)NxEn::Input::Mode::COUNT		, "Button", "Axis", "Mouse");
NX_ENUM_STRING(NxEn::Input::Axis		, (uint64)NxEn::Input::Axis::COUNT		, "MouseX", "MouseY", "ScrollX", "ScrollY");
NX_ENUM_STRING(NxEn::Input::Button	, (uint64)NxEn::Input::Button::COUNT	,
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
		union NX_ENGINE_API Binding
		{
			struct BindingButton
			{
				BindingButton(Button ButtonInput, State ButtonState);

				Button ButtonInput;
				State ButtonState;
			};

			Binding(Button ButtonInput, State ButtonState);
			Binding(Axis InputAxis);
			Binding(NxFr::Rectangle InputMouse);
			~Binding();

			Binding& operator=(BindingButton Other);
			Binding& operator=(Axis Other);
			Binding& operator=(NxFr::Rectangle Other);

			BindingButton InputButton;
			Axis InputAxis;
			NxFr::Rectangle InputMouse;
		};

		struct NX_ENGINE_API Trigger
		{
		public:
			Trigger(Button ButtonInput, State ButtonTarget, Modifier Modifiers = Modifier::Ignore);
			Trigger(Axis AxisInput, Modifier Modifiers = Modifier::Ignore);
			Trigger(NxFr::Rectangle MouseInput, Modifier Modifiers = Modifier::Ignore);
			Trigger(const Trigger& Other);
			~Trigger();

			Trigger& operator=(const Trigger& Other);

			Mode GetMode() const;
			Modifier GetModifiers() const;

			Button GetInputButton() const;
			State GetInputButtonState() const;
			Axis GetInputAxis() const;
			NxFr::Rectangle GetInputMouse() const;


		private:
			Mode Mode;
			Modifier Modifiers;
			Binding Input;
		};

		struct NX_ENGINE_API Action
		{
		public:
			Action(const Trigger& Input, const NxFr::Delegate<void()>& Callback);
			Action(Button ButtonInput, State ButtonTarget, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			Action(Axis AxisInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			Action(NxFr::Rectangle MouseInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			Action(const Action& Other);
			~Action();

			Action& operator=(const Action& Other);

			const Trigger& GetTrigger() const;
			const NxFr::Delegate<void()>& GetCallback() const;

		private:
			Trigger Input;
			NxFr::Delegate<void()> Callback;
		};

		struct NX_ENGINE_API Schema
		{
		public:
			Schema();
			~Schema();

			NxFr::Dictionary<NxFr::StringId, Action>& GetMapping();

		private:
			NxFr::Dictionary<NxFr::StringId, Action> Actions;
		};
	}
}
