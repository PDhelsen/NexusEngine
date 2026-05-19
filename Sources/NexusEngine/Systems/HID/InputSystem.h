#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	class InputSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, InputSystem)

		NX_ENGINE_API InputSystem();
		NX_ENGINE_API ~InputSystem();

		NX_ENGINE_API void Reset();

		NX_ENGINE_API void AddSchema(NxFr::StringId Id, Input::Schema* Schema);
		NX_ENGINE_API void RemoveSchema(NxFr::StringId Id);
		NX_ENGINE_API Input::Schema* GetSchema(NxFr::StringId Id);

		NX_ENGINE_API bool CheckButton(Input::Button Button, Input::State State = Input::State::Released) const;
		NX_ENGINE_API bool CheckAxis(Input::Axis Axis) const;
		NX_ENGINE_API bool CheckMouse() const;
		NX_ENGINE_API bool CheckModifier(Input::Modifier Modifier) const;

		NX_ENGINE_API Input::State GetButton(Input::Button Button) const;
		NX_ENGINE_API float GetAxis(Input::Axis Axis) const;
		NX_ENGINE_API NxFr::Vector2f GetMousePosition(bool Absolute = false) const;
		NX_ENGINE_API NxFr::Vector2f GetMouseDelta() const;
		NX_ENGINE_API Input::Modifier GetModifiers() const;

		NX_ENGINE_API NxFr::Event<Input::Button, Input::State>& GetOnButtonChange() { return OnButtonChange; }
		NX_ENGINE_API NxFr::Event<Input::Axis, float>& GetOnAxisChange() { return OnAxisChange; }
		NX_ENGINE_API NxFr::Event<NxFr::Vector2f>& GetOnMouseChange() { return OnMouseChange; }
		NX_ENGINE_API NxFr::Event<bool>& GetOnFocusChange() { return OnFocusChange; }
		NX_ENGINE_API NxFr::Event<>& GetOnPoll() { return OnPoll; }

		NX_ENGINE_API bool IsFocused() const { return Focused; }

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		NX_ENGINE_API void OnButtonChanged(Input::Button Button, Input::State State);
		NX_ENGINE_API void OnAxisChanged(Input::Axis Axis, float Delta);
		NX_ENGINE_API void OnMouseChanged(NxFr::Vector2f Position);
		NX_ENGINE_API void OnFocusChanged(bool Focus);

	private:
		NX_ENGINE_API void UpdateButtons();
		NX_ENGINE_API void UpdateAxises();
		NX_ENGINE_API void UpdateModifiers();

		NX_ENGINE_API void PollInputs();
		NX_ENGINE_API void TriggerActions();

	private:
		NxFr::Event<Input::Button, Input::State> OnButtonChange;
		NxFr::Event<Input::Axis, float> OnAxisChange;
		NxFr::Event<NxFr::Vector2f> OnMouseChange;
		NxFr::Event<bool> OnFocusChange;
		NxFr::Event<> OnPoll;

		class WindowSystem* Window;

		NxFr::Dictionary<NxFr::StringId, Input::Schema*> Schemas;

		NxFr::Array<Input::State, (uint64)Input::Button::COUNT> Buttons;
		NxFr::Array<float, (uint64)Input::Axis::COUNT> Axises;
		NxEn::Input::Modifier Modifiers;
		NxFr::Vector2f MousePosition;
		NxFr::Vector2f MouseDelta;
		bool Focused;

		bool DirtyFlagButtons;
		bool DirtyFlagAxises;
	};
}
