#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/InputSystem.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(InputSystem)

	InputSystem::InputSystem()
		: OnButtonChange(), OnAxisChange(), OnMouseChange(), Schemas(), Buttons(), Axises(), Mouse(-NxFr::Vector2f::One), Modifiers(), DirtyFlagButtons(true), DirtyFlagAxises(true)
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
		NxFr::ContainersUtils::Fill(Buttons, Input::State::Up);
		NxFr::ContainersUtils::Fill(Axises, 0.0f);
	}

	void InputSystem::AddSchema(NxFr::StringId Id, Input::Schema* Schema)
	{
		Schemas.AppendOrAssign(Id, Schema);
		NEXUS_LOG(Info, System, "Register Input schema: %s", Id.C());
	}

	void InputSystem::RemoveSchema(NxFr::StringId Id)
	{
		Schemas.Remove(Id);
		NEXUS_LOG(Info, System, "Unregister Input schema: %s", Id.C());
	}

	Input::Schema* InputSystem::GetSchema(NxFr::StringId Id)
	{
		return Schemas[Id];
	}

	Input::State InputSystem::GetButton(Input::Button Button) const
	{
		return Buttons[(uint64)Button];
	}

	float InputSystem::GetAxis(Input::Axis Axis) const
	{
		return Axises[(uint64)Axis];
	}

	NxFr::Vector2f InputSystem::GetMouse() const
	{
		return Mouse;
	}

	Input::Modifier InputSystem::GetModifiers() const
	{
		return Modifiers;
	}

	void InputSystem::OnInitialize()
	{
		System::OnInitialize();
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
		NxFr::Vector2f Delta = Position - Mouse;
		Axises[(uint64)Input::Axis::MouseX] = Delta.x;
		Axises[(uint64)Input::Axis::MouseY] = Delta.y;

		Mouse = Position;
		DirtyFlagAxises = true;
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
		NEXUS_PROFILE_FUNCTION();

		Glfw::PollInput();
		NEXUS_ASSERT(GetButton(Input::Button::Invalid) == Input::State::Up, System, "Unsupported Button pressed");
	}

	void InputSystem::TriggerActions()
	{
		NEXUS_PROFILE_FUNCTION();

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
				case Input::Mode::Mouse: Invoke = NxFr::ShapeUtility::Contains(Trigger.GetInputMouse(), Mouse); break;
				}

				if (Invoke)
				{
					Action.GetCallback().Invoke();
				}
			}
		}
	}
}
