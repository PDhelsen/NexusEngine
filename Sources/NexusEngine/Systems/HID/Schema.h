#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/HID/Input.h"

namespace NxEn
{
	namespace Input
	{
		union Binding
		{
			Binding(Button ButtonInput, State ButtonState);
			Binding(Axis InputAxis);
			Binding(NxFr::Rectangle InputMouse);
			~Binding();

			struct BindingButton
			{
				BindingButton(Button ButtonInput, State ButtonState);

				Button ButtonInput;
				State ButtonState;
			} InputButton;
			Axis InputAxis;
			NxFr::Rectangle InputMouse;
		};

		struct Trigger
		{
		public:
			NEXUS_ENGINE_API Trigger(Button ButtonInput, State ButtonTarget, Modifier Modifiers = Modifier::Ignore);
			NEXUS_ENGINE_API Trigger(Axis AxisInput, Modifier Modifiers = Modifier::Ignore);
			NEXUS_ENGINE_API Trigger(NxFr::Rectangle MouseInput, Modifier Modifiers = Modifier::Ignore);
			NEXUS_ENGINE_API ~Trigger();

			NEXUS_ENGINE_API Mode GetMode() const;
			NEXUS_ENGINE_API Modifier GetModifiers() const;

			NEXUS_ENGINE_API Button GetInputButton() const;
			NEXUS_ENGINE_API State GetInputButtonState() const;
			NEXUS_ENGINE_API Axis GetInputAxis() const;
			NEXUS_ENGINE_API NxFr::Rectangle GetInputMouse() const;


		private:
			Mode Mode;
			Modifier Modifiers;
			Binding Input;
		};

		struct Action
		{
		public:
			NEXUS_ENGINE_API Action(const Trigger& Input, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(Button ButtonInput, State ButtonTarget, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(Axis AxisInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API Action(NxFr::Rectangle MouseInput, Modifier Modifiers, const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API ~Action();

			NEXUS_ENGINE_API const Trigger& GetTrigger() const;
			NEXUS_ENGINE_API const NxFr::Delegate<void()>& GetCallback() const;

		private:
			Trigger Input;
			NxFr::Delegate<void()> Callback;
		};

		struct Schema
		{
		public:
			NEXUS_ENGINE_API Schema() = default;
			NEXUS_ENGINE_API ~Schema() = default;

			NEXUS_ENGINE_API NxFr::Dictionary<NxFr::StringId, Action>& GetMapping();

		private:
			NxFr::Dictionary<NxFr::StringId, Action> Actions;
		};
	}
}
