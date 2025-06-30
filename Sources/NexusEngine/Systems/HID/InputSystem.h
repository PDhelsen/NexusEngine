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

		NEXUS_ENGINE_API InputSystem();

		NEXUS_ENGINE_API void Reset();
		NEXUS_ENGINE_API Input::State GetButton(Input::Button Button) const;
		NEXUS_ENGINE_API bool IsButton(Input::Button Button, Input::State State, Input::Modifier Modifier = Input::Modifier::Ignore) const;

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		void OnButtonChanged(Input::Button Button, Input::State State);

		void UpdateState();
		void UpdateModifier();

	private:
		NxFr::Array<Input::State, (uint64)Input::Button::COUNT> States;
		Input::Modifier Modifiers;
		bool DirtyFlag;
	};
}
