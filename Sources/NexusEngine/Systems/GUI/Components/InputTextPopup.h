#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class NX_ENGINE_API InputTextPopup : public GUI::Popup
	{
	public:
		NX_OBJECT_DECLARATION(InputTextPopup)

		static InputTextPopup* GetInstance();

		void RegisterCallback(const NxFr::Delegate<void(NxFr::StringView)>& Callback);
		void UnregisterCallback();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnGui(float TimeStep) override;

	private:
		NxFr::String Input;
		NxFr::Delegate<void(NxFr::StringView)> Callback;
		GUI::Style Style;
	};
}
