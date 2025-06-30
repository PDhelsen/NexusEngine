#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/InputSystem.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(InputSystem)

		InputSystem::InputSystem()
		: OnButtonChange(), States(), Modifiers(Input::Modifier::None), DirtyFlag(true)
	{
		OnButtonChange += NxFr::Delegate<void(Input::Button, Input::State)>(this, &InputSystem::OnButtonChanged);
	}

	void InputSystem::Reset()
	{
		for (uint64 Index = 0; Index < (uint64)Input::Button::COUNT; ++Index)
		{
			States[Index] = Input::State::Up;
		}
	}

	Input::State InputSystem::GetButton(Input::Button Button) const
	{
		return States[(uint64)Button];
	}

	bool InputSystem::IsButton(Input::Button Button, Input::State State, Input::Modifier Modifier) const
	{
		if (Modifier == Input::Modifier::Ignore)
		{
			return GetButton(Button) == State;
		}

		return GetButton(Button) == State && Modifiers == Modifier;
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

		NEXUS_ASSERT(GetButton(Input::Button::Invalid) == Input::State::Up, Default, "Unsupported Button pressed");
	}

	void InputSystem::OnButtonChanged(Input::Button Button, Input::State State)
	{
		States[(uint64)Button] = State;
		DirtyFlag = true;
	}

	void InputSystem::UpdateState()
	{
		if (!DirtyFlag)
		{
			return;
		}

		for (uint64 Index = 0; Index < (uint64)Input::Button::COUNT; ++Index)
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

		Modifiers = Input::Enum::SetFlag(Modifiers, Input::Modifier::Shift, !(GetButton(Input::Button::LeftShift) == Input::State::Up && GetButton(Input::Button::RightShift) == Input::State::Up));
		Modifiers = Input::Enum::SetFlag(Modifiers, Input::Modifier::Control, !(GetButton(Input::Button::LeftControl) == Input::State::Up && GetButton(Input::Button::RightControl) == Input::State::Up));
		Modifiers = Input::Enum::SetFlag(Modifiers, Input::Modifier::Alt, !(GetButton(Input::Button::LeftAlt) == Input::State::Up && GetButton(Input::Button::RightAlt) == Input::State::Up));
	}
}
