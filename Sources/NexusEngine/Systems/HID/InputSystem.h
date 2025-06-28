#pragma once

#include "NexusEngine/Systems/System.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	class InputSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, InputSystem)

		NxFr::Event<Input::KeyCode, Input::State> OnKeyChange;

		NEXUS_ENGINE_API InputSystem();

		NEXUS_ENGINE_API void Reset();
		NEXUS_ENGINE_API Input::State GetState(Input::KeyCode Code) const;
		NEXUS_ENGINE_API bool IsState(Input::KeyCode Code, Input::State State, Input::Modifier Modifier = Input::Modifier::Ignore) const;

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		void OnKeyChanged(Input::KeyCode Code, Input::State State);

		void UpdateState();
		void UpdateModifier();

	private:
		NxFr::Array<Input::State, (uint64)Input::KeyCode::COUNT> States;
		Input::Modifier Modifiers;
		bool DirtyFlag;
	};
}
