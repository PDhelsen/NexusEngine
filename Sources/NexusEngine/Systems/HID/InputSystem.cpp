#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/InputSystem.h"

namespace NxEn
{
	static SettingVar<uint64>* SettingDoubleClickDistance = SettingVar<uint64>::Create("Settings", "DoubleClickDistance", 5);
	static SettingVar<double>* SettingDoubleClickTiming = SettingVar<double>::Create("Settings", "DoubleClickTiming", 0.25f);

	InputSystem::InputSystem()
		: OnButtonChange(), OnAxisChange(), OnMouseChange(), OnFocusChange(), OnPoll(),
		Buttons(), Axises(), Mouse(-NxFr::Vector2i::One, -NxFr::Vector2i::One), Click(Input::Button::Invalid, 0.0, NxFr::Vector2i::Zero), Modifiers(),
		Schemas(), Focused(false), DirtyFlagButtons(true), DirtyFlagAxises(true)
	{
		OnButtonChange += { this, &InputSystem::OnButtonChanged };
		OnAxisChange += { this, & InputSystem::OnAxisChanged };
		OnMouseChange += { this, & InputSystem::OnMouseChanged };
		OnFocusChange += { this, & InputSystem::OnFocusChanged };

		NxFr::ContainerUtility::Fill(Buttons, Input::State::Up);
		NxFr::ContainerUtility::Fill(Axises, 0.0f);
	}

	InputSystem::~InputSystem()
	{
		OnButtonChange -= { this, & InputSystem::OnButtonChanged };
		OnAxisChange -= { this, & InputSystem::OnAxisChanged };
		OnMouseChange -= { this, & InputSystem::OnMouseChanged };
		OnFocusChange -= { this, & InputSystem::OnFocusChanged };
	}

	void InputSystem::AddSchema(NxFr::StringId Id, Input::Schema* Schema)
	{
		NX_INSTUMENT_FUNCTION();

		Schemas.AppendOrAssign(Id, Schema);
		NX_LOG(Info, System, "Register Input schema: %s", Id.C());
	}

	void InputSystem::RemoveSchema(NxFr::StringId Id)
	{
		NX_INSTUMENT_FUNCTION();

		Schemas.Remove(Id);
		NX_LOG(Info, System, "Unregister Input schema: %s", Id.C());
	}

	void InputSystem::ExecuteAction(const Input::Action& Action) const
	{
		NX_INSTUMENT_FUNCTION();

		if (CheckTrigger(Action.Input))
		{
			Action.Callback.Invoke();
		}
	}

	bool InputSystem::CheckTrigger(const Input::Trigger& Trigger) const
	{
		bool IsTriggered = false;

		if (Trigger.Modifiers != Input::Modifier::Ignore && Trigger.Modifiers != Modifiers)
		{
			return IsTriggered;
		}

		switch (Trigger.Bindings.GetMode())
		{
		case Input::Mode::Button: IsTriggered = GetButton(Trigger.Bindings.GetButton().Key) == Trigger.Bindings.GetButton().Target; break;
		case Input::Mode::Axis: IsTriggered = GetAxis(Trigger.Bindings.GetAxis()) != 0.0f; break;
		case Input::Mode::Mouse: IsTriggered = NxFr::ShapeUtility::Contains(Trigger.Bindings.GetMouse(), Mouse.Position); break;
		}

		return IsTriggered;
	}

	bool InputSystem::CheckButton(Input::Button Button, Input::State State) const
	{
		return GetButton(Button) == State;
	}

	bool InputSystem::CheckDoubleClick(Input::Button Button) const
	{
		return Click.Key == Button && Click.Time == 0.0 && Click.Position == NxFr::Vector2i::Zero;
	}

	bool InputSystem::CheckAxis(Input::Axis Axis) const
	{
		return GetAxis(Axis) != 0.0f;
	}

	bool InputSystem::CheckMouse() const
	{
		return GetMouseDelta() != NxFr::Vector2i::Zero;
	}

	bool InputSystem::CheckModifier(Input::Modifier Modifier) const
	{
		return NxFr::Enum::CheckFlag(Modifiers, Modifier);
	}

	Input::State InputSystem::GetButton(Input::Button Button) const
	{
		return Buttons[(uint64)Button];
	}

	float InputSystem::GetAxis(Input::Axis Axis) const
	{
		return Axises[(uint64)Axis];
	}

	NxFr::Vector2i InputSystem::GetMousePosition() const
	{
		return Mouse.Position;
	}

	NxFr::Vector2i InputSystem::GetMouseDelta() const
	{
		return Mouse.Delta;
	}

	Input::Modifier InputSystem::GetModifiers() const
	{
		return Modifiers;
	}

	bool InputSystem::IsFocused() const
	{
		return Focused;
	}

	bool InputSystem::IsWriting() const
	{
		return ImGui::GetIO().WantTextInput;
	}

	void InputSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		UpdateButtons();
		UpdateAxises();
		UpdateModifiers();

		DirtyFlagButtons = false;
		DirtyFlagAxises = false;

		PollInputs();
		TriggerActions();
	}

	void InputSystem::OnButtonChanged(Input::Button Button, Input::State State)
	{
		Buttons[(uint64)Button] = State;
		DirtyFlagButtons = true;

		if (State == Input::State::Pressed)
		{
			double Now = NxFr::Time::ProcessorTick();
			bool DoubleClick =
				Click.Key == Button &&
				Now - Click.Time < SettingDoubleClickTiming->GetValue() &&
				NxFr::VectorUtility::Distance(Click.Position, Mouse.Position) < SettingDoubleClickDistance->GetValue();
			Click = DoubleClick ?
				Input::ClickState(Button, 0.0, NxFr::Vector2i::Zero) :
				Input::ClickState(Button, Now, Mouse.Position);
		}
	}

	void InputSystem::OnAxisChanged(Input::Axis Axis, float Delta)
	{
		Axises[(uint64)Axis] = Delta;
		DirtyFlagAxises = true;
	}

	void InputSystem::OnMouseChanged(NxFr::Vector2i Position)
	{
		Mouse.Delta = Position - Mouse.Position;
		Axises[(uint64)Input::Axis::MouseX] = Mouse.Delta.x;
		Axises[(uint64)Input::Axis::MouseY] = Mouse.Delta.y;
		Mouse.Position = Position;
		DirtyFlagAxises = true;
	}

	void InputSystem::OnFocusChanged(bool Focus)
	{
		Focused = Focus;
	}

	void InputSystem::UpdateButtons()
	{
		if (!DirtyFlagButtons)
		{
			return;
		}

		for (uint64 Index = 0; Index < (uint64)Input::Button::COUNT; ++Index)
		{
			switch (Buttons[Index])
			{
				case Input::State::Up: break;
				case Input::State::Down: break;
				case Input::State::Released: Buttons[Index] = Input::State::Up; break;
				case Input::State::Pressed: Buttons[Index] = Input::State::Down; break;
			}
		}

		if (Click.Key != Input::Button::Invalid && Click.Time == 0.0 && Click.Position == NxFr::Vector2i::Zero)
		{
			Click = Input::ClickState(Input::Button::Invalid, 0.0, NxFr::Vector2i::Zero);
		}
	}

	void InputSystem::UpdateAxises()
	{
		if (!DirtyFlagAxises)
		{
			return;
		}

		for (uint64 Index = 0; Index < (uint64)Input::Axis::COUNT; ++Index)
		{
			Axises[Index] = 0.0f;
		}

		Mouse.Delta = NxFr::Vector2i::Zero;
	}

	void InputSystem::UpdateModifiers()
	{
		if (!DirtyFlagButtons)
		{
			return;
		}

		Modifiers = NxFr::Enum::SetFlag(Modifiers, Input::Modifier::Ctrl, GetButton(Input::Button::LeftControl) != Input::State::Up || GetButton(Input::Button::RightControl) != Input::State::Up);
		Modifiers = NxFr::Enum::SetFlag(Modifiers, Input::Modifier::Shift, GetButton(Input::Button::LeftShift) != Input::State::Up || GetButton(Input::Button::RightShift) != Input::State::Up);
		Modifiers = NxFr::Enum::SetFlag(Modifiers, Input::Modifier::Alt, GetButton(Input::Button::LeftAlt) != Input::State::Up || GetButton(Input::Button::RightAlt) != Input::State::Up);
	}

	void InputSystem::PollInputs()
	{
		NX_INSTUMENT_SCOPE("Poll");

		OnPoll();
		NX_ASSERT_RETURN(GetButton(Input::Button::Invalid) == Input::State::Up, , System, "Unsupported Button pressed");
	}

	void InputSystem::TriggerActions()
	{
		NX_INSTUMENT_SCOPE("Actions");

		for (auto& [Id, Schema] : Schemas)
		{
			NX_INSTUMENT_SCOPE(Id);

			for (auto& [Tag, Action] : Schema->Mapping)
			{
				NX_INSTUMENT_SCOPE(Tag);

				ExecuteAction(Action);
			}
		}
	}
}
