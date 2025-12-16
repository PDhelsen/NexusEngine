#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	class InputSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, InputSystem)

		NEXUS_ENGINE_API InputSystem();
		NEXUS_ENGINE_API ~InputSystem();

		NEXUS_ENGINE_API void Reset();

		NEXUS_ENGINE_API void AddSchema(NxFr::StringId Id, Input::Schema* Schema);
		NEXUS_ENGINE_API void RemoveSchema(NxFr::StringId Id);
		NEXUS_ENGINE_API Input::Schema* GetSchema(NxFr::StringId Id);

		NEXUS_ENGINE_API bool CheckButton(Input::Button Button, Input::State State = Input::State::Released) const;
		NEXUS_ENGINE_API bool CheckAxis(Input::Axis Axis) const;
		NEXUS_ENGINE_API bool CheckMouse() const;
		NEXUS_ENGINE_API bool CheckModifier(Input::Modifier Modifier) const;

		NEXUS_ENGINE_API Input::State GetButton(Input::Button Button) const;
		NEXUS_ENGINE_API float GetAxis(Input::Axis Axis) const;
		NEXUS_ENGINE_API NxFr::Vector2f GetMousePosition(bool Absolute = false) const;
		NEXUS_ENGINE_API NxFr::Vector2f GetMouseDelta() const;
		NEXUS_ENGINE_API Input::Modifier GetModifiers() const;

		NEXUS_ENGINE_API NxFr::Event<Input::Button, Input::State>& GetOnButtonChange() { return OnButtonChange; }
		NEXUS_ENGINE_API NxFr::Event<Input::Axis, float>& GetOnAxisChange() { return OnAxisChange; }
		NEXUS_ENGINE_API NxFr::Event<NxFr::Vector2f>& GetOnMouseChange() { return OnMouseChange; }

		NEXUS_ENGINE_API bool IsMouseOverWindow() const { return MousePosition != -NxFr::Vector2f::One; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		NEXUS_ENGINE_API void OnButtonChanged(Input::Button Button, Input::State State);
		NEXUS_ENGINE_API void OnAxisChanged(Input::Axis Axis, float Delta);
		NEXUS_ENGINE_API void OnMouseChanged(NxFr::Vector2f Position);

	private:
		NEXUS_ENGINE_API void UpdateButtons();
		NEXUS_ENGINE_API void UpdateAxises();
		NEXUS_ENGINE_API void UpdateModifiers();

		NEXUS_ENGINE_API void PollInputs();
		NEXUS_ENGINE_API void TriggerActions();

	private:
		NxFr::Event<Input::Button, Input::State> OnButtonChange;
		NxFr::Event<Input::Axis, float> OnAxisChange;
		NxFr::Event<NxFr::Vector2f> OnMouseChange;

		class WindowSystem* Window;

		NxFr::Dictionary<NxFr::StringId, Input::Schema*> Schemas;

		NxFr::Array<Input::State, (uint64)Input::Button::COUNT> Buttons;
		NxFr::Array<float, (uint64)Input::Axis::COUNT> Axises;
		NxEn::Input::Modifier Modifiers;
		NxFr::Vector2f MousePosition;
		NxFr::Vector2f MouseDelta;

		bool DirtyFlagButtons;
		bool DirtyFlagAxises;
	};
}
