#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/Schema.h"

namespace NxEn
{
	namespace Input
	{
		Binding::BindingButton::BindingButton(Button ButtonInput, State ButtonState)
			: ButtonInput(ButtonInput), ButtonState(ButtonState)
		{
		}

		Binding::Binding(Button ButtonInput, State ButtonState)
			: InputButton(ButtonInput, ButtonState)
		{
		}

		Binding::Binding(Axis InputAxis)
			: InputAxis(InputAxis)
		{
		}

		Binding::Binding(NxFr::Rectangle InputMouse)
			: InputMouse(InputMouse)
		{
		}

		Binding::~Binding()
		{
		}

		Trigger::Trigger(Button ButtonInput, State ButtonTarget, Modifier Modifiers)
			: Mode(Mode::Button), Modifiers(Modifiers), Input(ButtonInput, ButtonTarget)
		{
		}

		Trigger::Trigger(Axis AxisInput, Modifier Modifiers)
			: Mode(Mode::Axis), Modifiers(Modifiers), Input(AxisInput)
		{
		}

		Trigger::Trigger(NxFr::Rectangle MouseInput, Modifier Modifiers)
			: Mode(Mode::Mouse), Modifiers(Modifiers), Input(MouseInput)
		{
		}

		Trigger::~Trigger()
		{
		}

		Mode Trigger::GetMode() const
		{
			return Mode;
		}

		Modifier Trigger::GetModifiers() const
		{
			return Modifiers;
		}

		Button Trigger::GetInputButton() const
		{
			NEXUS_ASSERT(Mode == Mode::Button, Default, "Input mode is not set to button");
			return Input.InputButton.ButtonInput;
		}

		State Trigger::GetInputButtonState() const
		{
			NEXUS_ASSERT(Mode == Mode::Button, Default, "Input mode is not set to button");
			return Input.InputButton.ButtonState;
		}

		Axis Trigger::GetInputAxis() const
		{
			NEXUS_ASSERT(Mode == Mode::Axis, Default, "Input mode is not set to axis");
			return Input.InputAxis;
		}

		NxFr::Rectangle Trigger::GetInputMouse() const
		{
			NEXUS_ASSERT(Mode == Mode::Mouse, Default, "Input mode is not set to mouse");
			return Input.InputMouse;
		}

		Action::Action(const Trigger& Input, const NxFr::Delegate<void()>& Callback)
			: Input(Input), Callback(Callback)
		{
		}

		Action::Action(Button ButtonInput, State ButtonTarget, Modifier Modifiers, const NxFr::Delegate<void()>& Callback)
			: Input(ButtonInput, ButtonTarget, Modifiers), Callback(Callback)
		{
		}

		Action::Action(Axis AxisInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback)
			: Input(AxisInput, Modifiers), Callback(Callback)
		{
		}

		Action::Action(NxFr::Rectangle MouseInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback)
			: Input(MouseInput, Modifiers), Callback(Callback)
		{
		}

		Action::~Action()
		{
		}

		const Trigger& Action::GetTrigger() const
		{
			return Input;
		}

		const NxFr::Delegate<void()>& Action::GetCallback() const
		{
			return Callback;
		}

		NxFr::Dictionary<NxFr::StringId, Action>& Schema::GetMapping()
		{
			return Actions;
		}
	}
}
