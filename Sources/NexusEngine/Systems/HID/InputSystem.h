#pragma once

#include "NexusEngine/Systems/System.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	class InputSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, InputSystem)

		NxFr::Event<Input::Button, Input::State> OnButtonChange;
		NxFr::Event<Input::Axis, float> OnAxisChange;
		NxFr::Event<NxFr::Vector2f> OnMouseChange;

		NEXUS_ENGINE_API InputSystem();

		NEXUS_ENGINE_API void Reset();
		NEXUS_ENGINE_API Input::State GetButton(Input::Button Button) const;
		NEXUS_ENGINE_API float GetAxis(Input::Axis Axis) const;
		NEXUS_ENGINE_API NxFr::Vector2f GetMouse() const;

		bool IsMouseOverWindow() const { return Mouse != -NxFr::Vector2f::One; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		NEXUS_ENGINE_API void OnButtonChanged(Input::Button Button, Input::State State);
		NEXUS_ENGINE_API void OnAxisChanged(Input::Axis Axis, float Delta);
		NEXUS_ENGINE_API void OnMouseChanged(NxFr::Vector2f Position);

		NEXUS_ENGINE_API void UpdateButtons();
		NEXUS_ENGINE_API void UpdateAxises();

	private:
		NxFr::Array<Input::State, (uint64)Input::Button::COUNT> Buttons;
		NxFr::Array<float, (uint64)Input::Axis::COUNT> Axises;
		NxFr::Vector2f Mouse;
		bool DirtyFlagButtons;
		bool DirtyFlagAxises;
	};
}
