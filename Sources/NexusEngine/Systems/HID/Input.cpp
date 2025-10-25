#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/Input.h"

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

		Binding& Binding::operator=(BindingButton Other)
		{
			InputButton = Other;
			return *this;
		}

		Binding& Binding::operator=(Axis Other)
		{
			InputAxis = Other;
			return *this;
		}

		Binding& Binding::operator=(NxFr::Rectangle Other)
		{
			InputMouse = Other;
			return *this;
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

		Trigger::Trigger(const Trigger& Other)
			: Mode(Other.Mode), Modifiers(Other.Modifiers), Input((Axis)0)
		{
			switch (Mode)
			{
			case NxEn::Input::Mode::Button: Input.InputButton = Other.Input.InputButton; break;
			case NxEn::Input::Mode::Axis: Input.InputAxis = Other.Input.InputAxis; break;
			case NxEn::Input::Mode::Mouse: Input.InputMouse = Other.Input.InputMouse; break;
			}
		}

		Trigger::~Trigger()
		{
		}

		Trigger& Trigger::operator=(const Trigger& Other)
		{
			Mode = Other.Mode;
			Modifiers = Other.Modifiers;
			switch (Mode)
			{
			case NxEn::Input::Mode::Button: Input.InputButton = Other.Input.InputButton; break;
			case NxEn::Input::Mode::Axis: Input.InputAxis = Other.Input.InputAxis; break;
			case NxEn::Input::Mode::Mouse: Input.InputMouse = Other.Input.InputMouse; break;
			}

			return *this;
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

		Action::Action(const Action& Other)
			: Input(Other.Input), Callback(Other.Callback)
		{
		}

		Action::~Action()
		{
		}

		Action& Action::operator=(const Action& Other)
		{
			Input = Other.Input;
			Callback = Other.Callback;
			return *this;
		}

		const Trigger& Action::GetTrigger() const
		{
			return Input;
		}

		const NxFr::Delegate<void()>& Action::GetCallback() const
		{
			return Callback;
		}

		Schema::Schema()
			: Actions()
		{
		}

		Schema::~Schema()
		{
		}

		NxFr::Dictionary<NxFr::StringId, Action>& Schema::GetMapping()
		{
			return Actions;
		}
	}
}
