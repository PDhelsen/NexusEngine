#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/InputSystem.h"

namespace NxEn
{
	NX_OBJECT_IMPLEMENTATION(InputSystem)

	InputSystem::InputSystem()
		: OnButtonChange(), OnAxisChange(), OnMouseChange(), Window(nullptr), Schemas(), Buttons(), Axises(), Modifiers(), MousePosition(-NxFr::Vector2f::One), MouseDelta(-NxFr::Vector2f::One), Focused(false), DirtyFlagButtons(true), DirtyFlagAxises(true)
	{
		OnButtonChange += NxFr::Delegate<void(Input::Button, Input::State)>(this, &InputSystem::OnButtonChanged);
		OnAxisChange += NxFr::Delegate<void(Input::Axis, float)>(this, &InputSystem::OnAxisChanged);
		OnMouseChange += NxFr::Delegate<void(NxFr::Vector2f)>(this, &InputSystem::OnMouseChanged);

		Reset();
	}

	InputSystem::~InputSystem()
	{
		OnButtonChange -= NxFr::Delegate<void(Input::Button, Input::State)>(this, &InputSystem::OnButtonChanged);
		OnAxisChange -= NxFr::Delegate<void(Input::Axis, float)>(this, &InputSystem::OnAxisChanged);
		OnMouseChange -= NxFr::Delegate<void(NxFr::Vector2f)>(this, &InputSystem::OnMouseChanged);
	}

	void InputSystem::Reset()
	{
		NxFr::ContainerUtility::Fill(Buttons, Input::State::Up);
		NxFr::ContainerUtility::Fill(Axises, 0.0f);
	}

	void InputSystem::AddSchema(NxFr::StringId Id, Input::Schema* Schema)
	{
		Schemas.AppendOrAssign(Id, Schema);
		NX_LOG(Info, System, "Register Input schema: %s", Id.C());
	}

	void InputSystem::RemoveSchema(NxFr::StringId Id)
	{
		Schemas.Remove(Id);
		NX_LOG(Info, System, "Unregister Input schema: %s", Id.C());
	}

	Input::Schema* InputSystem::GetSchema(NxFr::StringId Id)
	{
		return Schemas[Id];
	}

	bool InputSystem::CheckButton(Input::Button Button, Input::State State) const
	{
		return GetButton(Button) == State;
	}

	bool InputSystem::CheckAxis(Input::Axis Axis) const
	{
		return GetAxis(Axis) != 0.0f;
	}

	bool InputSystem::CheckMouse() const
	{
		return IsFocused() && GetMouseDelta() != NxFr::Vector2f::Zero;
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

	NxFr::Vector2f InputSystem::GetMousePosition(bool Absolute) const
	{
		return Absolute ?
			MousePosition + (NxFr::Vector2f)Window->GetWindow().GetPosition() :
			MousePosition;
	}

	NxFr::Vector2f InputSystem::GetMouseDelta() const
	{
		return MouseDelta;
	}

	Input::Modifier InputSystem::GetModifiers() const
	{
		return Modifiers;
	}

	void InputSystem::OnInitialize()
	{
		System::OnInitialize();

		Window = NxEn::Application::GetSystem<NxEn::WindowSystem>();
	}

	void InputSystem::OnShutdown()
	{
		System::OnShutdown();
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
		if (ImGui::GetIO().WantTextInput)
		{
			return;
		}

		Buttons[(uint64)Button] = State;
		DirtyFlagButtons = true;
	}

	void InputSystem::OnAxisChanged(Input::Axis Axis, float Delta)
	{
		Axises[(uint64)Axis] = Delta;
		DirtyFlagAxises = true;
	}

	void InputSystem::OnMouseChanged(NxFr::Vector2f Position)
	{
		MouseDelta = Position - MousePosition;
		Axises[(uint64)Input::Axis::MouseX] = MouseDelta.x;
		Axises[(uint64)Input::Axis::MouseY] = MouseDelta.y;

		MousePosition = Position;
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
				case Input::State::Up:
				case Input::State::Released:
					Buttons[Index] = Input::State::Up;
				break;
				case Input::State::Down:
				case Input::State::Pressed:
					Buttons[Index] = Input::State::Down;
					Buttons[Index] = Input::State::Down;
				break;
			}
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

		MouseDelta = NxFr::Vector2f::Zero;
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
		NX_INSTUMENT_FUNCTION();

		OnPoll();
		NX_ASSERT(GetButton(Input::Button::Invalid) == Input::State::Up, System, "Unsupported Button pressed");
	}

	void InputSystem::TriggerActions()
	{
		NX_INSTUMENT_FUNCTION();

		for (auto& [Id, Schema] : Schemas)
		{
			for (auto& [Tag, Action] : Schema->GetMapping())
			{
				const Input::Trigger& Trigger = Action.GetTrigger();
				if (Trigger.GetModifiers() != Input::Modifier::Ignore && Trigger.GetModifiers() != Modifiers)
				{
					continue;
				}

				bool Invoke = false;
				switch (Trigger.GetMode())
				{
				case Input::Mode::Button: Invoke = GetButton(Trigger.GetInputButton()) == Trigger.GetInputButtonState(); break;
				case Input::Mode::Axis: Invoke = GetAxis(Trigger.GetInputAxis()) != 0.0f; break;
				case Input::Mode::Mouse: Invoke = NxFr::ShapeUtility::Contains(Trigger.GetInputMouse(), MousePosition); break;
				}

				if (Invoke)
				{
					Action.GetCallback().Invoke();
				}
			}
		}
	}
}
