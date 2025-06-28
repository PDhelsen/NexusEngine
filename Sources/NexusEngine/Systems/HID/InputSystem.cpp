#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/InputSystem.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(InputSystem)

		InputSystem::InputSystem()
		: OnKeyChange(), States(), Modifiers(Input::Modifier::None), DirtyFlag(true)
	{
		OnKeyChange += NxFr::Delegate<void(Input::KeyCode, Input::State)>(this, &InputSystem::OnKeyChanged);
	}

	void InputSystem::Reset()
	{
		for (uint64 Index = 0; Index < (uint64)Input::KeyCode::COUNT; ++Index)
		{
			States[Index] = Input::State::Up;
		}
	}

	Input::State InputSystem::GetState(Input::KeyCode Code) const
	{
		return States[(uint64)Code];
	}

	bool InputSystem::IsState(Input::KeyCode Code, Input::State State, Input::Modifier Modifier) const
	{
		if (Modifier == Input::Modifier::Ignore)
		{
			return GetState(Code) == State;
		}

		return GetState(Code) == State && Modifiers == Modifier;
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

		UpdateState();
		UpdateModifier();
		DirtyFlag = false;

		Glfw::PollInput();

		NEXUS_ASSERT(GetState(Input::KeyCode::Invalid) == Input::State::Up, Default, "Unsupported key pressed");
	}

	void InputSystem::OnKeyChanged(Input::KeyCode Code, Input::State State)
	{
		States[(uint64)Code] = State;
		DirtyFlag = true;
	}

	void InputSystem::UpdateState()
	{
		if (!DirtyFlag)
		{
			return;
		}

		for (uint64 Index = 0; Index < (uint64)Input::KeyCode::COUNT; ++Index)
		{
			switch (States[Index])
			{
				case Input::State::Up:
				case Input::State::Released:
					States[Index] = Input::State::Up;
				break;
				case Input::State::Pressed:
					States[Index] = Input::State::Down;
					States[Index] = Input::State::Down;
				break;
			}
		}
	}

	void InputSystem::UpdateModifier()
	{
		if (!DirtyFlag)
		{
			return;
		}

		Modifiers = Input::Enum::SetFlag(Modifiers, Input::Modifier::Shift, !(GetState(Input::KeyCode::LeftShift) == Input::State::Up && GetState(Input::KeyCode::RightShift) == Input::State::Up));
		Modifiers = Input::Enum::SetFlag(Modifiers, Input::Modifier::Control, !(GetState(Input::KeyCode::LeftControl) == Input::State::Up && GetState(Input::KeyCode::RightControl) == Input::State::Up));
		Modifiers = Input::Enum::SetFlag(Modifiers, Input::Modifier::Alt, !(GetState(Input::KeyCode::LeftAlt) == Input::State::Up && GetState(Input::KeyCode::RightAlt) == Input::State::Up));
	}
}
