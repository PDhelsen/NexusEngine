#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	namespace Input
	{
		ButtonState::ButtonState(Button Key, State Target)
			: Key(Key), Target(Target)
		{
		}

		MouseState::MouseState(NxFr::Vector2i Position, NxFr::Vector2i Delta)
			: Position(Position), Delta(Delta)
		{
		}

		Binding::Binding(Button ButtonKey, State ButtonTarget)
			: Mode(Mode::Button), Value({ Button::Invalid, State::COUNT })
		{
			Value.Btn = { ButtonKey, ButtonTarget };
		}

		Binding::Binding(Axis AxisValue)
			: Mode(Mode::Axis), Value({ Button::Invalid, State::COUNT })
		{
			Value.Axs = AxisValue;
		}

		Binding::Binding(NxFr::Rectangle MouseValue)
			: Mode(Mode::Mouse), Value({ Button::Invalid, State::COUNT })
		{
			Value.Mse = MouseValue;
		}

		Binding::Binding(const Binding& Other)
			: Mode(Other.Mode), Value({ Button::Invalid, State::COUNT })
		{
			switch (Mode)
			{
			case NxEn::Input::Mode::Button: Value.Btn = Other.Value.Btn; break;
			case NxEn::Input::Mode::Axis: Value.Axs = Other.Value.Axs; break;
			case NxEn::Input::Mode::Mouse:Value.Mse = Other.Value.Mse; break;
			}
		}

		Binding::~Binding()
		{
		}

		Binding& Binding::operator=(const Binding& Other)
		{
			Mode = Other.Mode;
			switch (Mode)
			{
			case NxEn::Input::Mode::Button: Value.Btn = Other.Value.Btn; break;
			case NxEn::Input::Mode::Axis: Value.Axs = Other.Value.Axs; break;
			case NxEn::Input::Mode::Mouse:Value.Mse = Other.Value.Mse; break;
			}

			return *this;
		}

		ButtonState& Binding::GetButton()
		{
			NX_ASSERT(Mode == Mode::Button, Default, "Binding is not in Button mode");
			return Value.Btn;
		}

		const ButtonState& Binding::GetButton() const
		{
			NX_ASSERT(Mode == Mode::Button, Default, "Binding is not in Button mode");
			return Value.Btn;
		}

		void Binding::SetButton(ButtonState ButtonValue)
		{
			Mode = Mode::Button;
			Value.Btn = ButtonValue;
		}

		Axis& Binding::GetAxis()
		{
			NX_ASSERT(Mode == Mode::Axis, Default, "Binding is not in Axis mode");
			return Value.Axs;
		}

		const Axis& Binding::GetAxis() const
		{
			NX_ASSERT(Mode == Mode::Axis, Default, "Binding is not in Axis mode");
			return Value.Axs;
		}

		void Binding::SetAxis(Axis AxisValue)
		{
			Mode = Mode::Axis;
			Value.Axs = AxisValue;
		}

		NxFr::Rectangle& Binding::GetMouse()
		{
			NX_ASSERT(Mode == Mode::Mouse, Default, "Binding is not in Mouse mode");
			return Value.Mse;
		}

		const NxFr::Rectangle& Binding::GetMouse() const
		{
			NX_ASSERT(Mode == Mode::Mouse, Default, "Binding is not in Mouse mode");
			return Value.Mse;
		}

		void Binding::SetMouse(NxFr::Rectangle MouseValue)
		{
			Mode = Mode::Mouse;
			Value.Mse = MouseValue;
		}

		Trigger::Trigger(Button ButtonKey, State ButtonTarget, Modifier Modifiers)
			: Bindings(ButtonKey, ButtonTarget), Modifiers(Modifiers)
		{
		}

		Trigger::Trigger(Axis AxisValue, Modifier Modifiers)
			: Bindings(AxisValue), Modifiers(Modifiers)
		{
		}

		Trigger::Trigger(NxFr::Rectangle MouseValue, Modifier Modifiers)
			: Bindings(MouseValue), Modifiers(Modifiers)
		{
		}

		Trigger::~Trigger()
		{
		}

		Action::Action(Button ButtonKey, State ButtonTarget, Modifier Modifiers, const NxFr::Delegate<void()>& Callback)
			: Input(ButtonKey, ButtonTarget, Modifiers), Callback(Callback)
		{
		}

		Action::Action(Axis AxisValue, Modifier Modifiers, const NxFr::Delegate<void()>& Callback)
			: Input(AxisValue, Modifiers), Callback(Callback)
		{
		}

		Action::Action(NxFr::Rectangle MouseValue, Modifier Modifiers, const NxFr::Delegate<void()>& Callback)
			: Input(MouseValue, Modifiers), Callback(Callback)
		{
		}

		Action::Action(const Trigger& Input, const NxFr::Delegate<void()>& Callback)
			: Input(Input), Callback(Callback)
		{
		}

		Action::~Action()
		{
		}

		Schema::Schema()
			: Mapping()
		{
		}

		Schema::~Schema()
		{
		}
	}
}
