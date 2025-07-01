#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/InputSystem.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(InputSystem)

	InputSystem::InputSystem()
		: OnButtonChange(), OnAxisChange(), OnMouseChange(), Buttons(), Axises(), Mouse(-NxFr::Vector2f::One), DirtyFlagButtons(true), DirtyFlagAxises(true)
	{
		OnButtonChange += NxFr::Delegate<void(Input::Button, Input::State)>(this, &InputSystem::OnButtonChanged);
		OnAxisChange += NxFr::Delegate<void(Input::Axis, float)>(this, &InputSystem::OnAxisChanged);
		OnMouseChange += NxFr::Delegate<void(NxFr::Vector2f)>(this, &InputSystem::OnMouseChanged);

		Reset();
	}

	void InputSystem::Reset()
	{
		for (uint64 Index = 0; Index < (uint64)Input::Button::COUNT; ++Index)
		{
			Buttons[Index] = Input::State::Up;
		}

		for (uint64 Index = 0; Index < (uint64)Input::Axis::COUNT; ++Index)
		{
			Axises[Index] = 0.0f;
		}
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

		Glfw::PollInput();

		NEXUS_ASSERT(GetButton(Input::Button::Invalid) == Input::State::Up, Default, "Unsupported Button pressed");
	}

	void InputSystem::OnButtonChanged(Input::Button Button, Input::State State)
	{
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

		DirtyFlagButtons = false;
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

		DirtyFlagAxises = false;
	}
}
