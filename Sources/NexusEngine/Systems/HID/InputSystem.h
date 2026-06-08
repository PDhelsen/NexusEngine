#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
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

		void AddSchema(NxFr::StringId Id, Input::Schema* Schema);
		void RemoveSchema(NxFr::StringId Id);
		void ExecuteAction(const Input::Action& Action) const;

		bool CheckTrigger(const Input::Trigger& Trigger) const;
		bool CheckButton(Input::Button Button, Input::State State = Input::State::Released) const;
		bool CheckDoubleClick(Input::Button Button) const;
		bool CheckAxis(Input::Axis Axis) const;
		bool CheckMouse() const;
		bool CheckModifier(Input::Modifier Modifier) const;

		Input::State GetButton(Input::Button Button) const;
		float GetAxis(Input::Axis Axis) const;
		NxFr::Vector2i GetMousePosition() const;
		NxFr::Vector2i GetMouseDelta() const;
		Input::Modifier GetModifiers() const;

		bool IsFocused() const;
		bool IsWriting() const;

		NxFr::Event<Input::Button, Input::State>& GetOnButtonChange() { return OnButtonChange; }
		NxFr::Event<Input::Axis, float>& GetOnAxisChange() { return OnAxisChange; }
		NxFr::Event<NxFr::Vector2i>& GetOnMouseChange() { return OnMouseChange; }
		NxFr::Event<bool>& GetOnFocusChange() { return OnFocusChange; }
		NxFr::Event<>& GetOnPoll() { return OnPoll; }

	protected:
		void OnTick(float TimeStep = 0.0f) override;

		void OnButtonChanged(Input::Button Button, Input::State State);
		void OnAxisChanged(Input::Axis Axis, float Delta);
		void OnMouseChanged(NxFr::Vector2i Position);
		void OnFocusChanged(bool Focus);

	private:
		void UpdateButtons();
		void UpdateAxises();
		void UpdateModifiers();
		void PollInputs();
		void TriggerActions();

		NxFr::Event<Input::Button, Input::State> OnButtonChange;
		NxFr::Event<Input::Axis, float> OnAxisChange;
		NxFr::Event<NxFr::Vector2i> OnMouseChange;
		NxFr::Event<bool> OnFocusChange;
		NxFr::Event<> OnPoll;

		NxFr::Array<Input::State, (uint64)Input::Button::COUNT> Buttons;
		NxFr::Array<float, (uint64)Input::Axis::COUNT> Axises;
		Input::MouseState Mouse;
		Input::ClickState Click;
		NxEn::Input::Modifier Modifiers;

		NxFr::Dictionary<NxFr::StringId, Input::Schema*> Schemas;
		bool Focused;
		bool DirtyFlagButtons;
		bool DirtyFlagAxises;
	};
}
