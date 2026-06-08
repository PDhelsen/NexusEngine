#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	class NX_ENGINE_API InputSystem : public System
	{
	public:
		NX_OBJECT(InputSystem)

		InputSystem();
		~InputSystem();

		void Reset();

		void AddSchema(NxFr::StringId Id, Input::Schema* Schema);
		void RemoveSchema(NxFr::StringId Id);
		Input::Schema* GetSchema(NxFr::StringId Id);

		bool CheckButton(Input::Button Button, Input::State State = Input::State::Released) const;
		bool CheckAxis(Input::Axis Axis) const;
		bool CheckMouse() const;
		bool CheckModifier(Input::Modifier Modifier) const;

		Input::State GetButton(Input::Button Button) const;
		float GetAxis(Input::Axis Axis) const;
		NxFr::Vector2f GetMousePosition() const;
		NxFr::Vector2f GetMouseDelta() const;
		Input::Modifier GetModifiers() const;

		NxFr::Event<Input::Button, Input::State>& GetOnButtonChange() { return OnButtonChange; }
		NxFr::Event<Input::Axis, float>& GetOnAxisChange() { return OnAxisChange; }
		NxFr::Event<NxFr::Vector2f>& GetOnMouseChange() { return OnMouseChange; }
		NxFr::Event<bool>& GetOnFocusChange() { return OnFocusChange; }
		NxFr::Event<>& GetOnPoll() { return OnPoll; }

		bool IsFocused() const { return Focused; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;
		void OnButtonChanged(Input::Button Button, Input::State State);
		void OnAxisChanged(Input::Axis Axis, float Delta);
		void OnMouseChanged(NxFr::Vector2f Position);
		void OnFocusChanged(bool Focus);

	private:
		void UpdateButtons();
		void UpdateAxises();
		void UpdateModifiers();

		void PollInputs();
		void TriggerActions();

	private:
		NxFr::Event<Input::Button, Input::State> OnButtonChange;
		NxFr::Event<Input::Axis, float> OnAxisChange;
		NxFr::Event<NxFr::Vector2f> OnMouseChange;
		NxFr::Event<bool> OnFocusChange;
		NxFr::Event<> OnPoll;

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
