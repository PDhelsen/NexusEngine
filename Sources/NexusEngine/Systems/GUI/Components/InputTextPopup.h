#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class InputTextPopup : public GUI::Popup
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, InputTextPopup)

		NX_ENGINE_API static InputTextPopup* GetInstance();

		NX_ENGINE_API void RegisterCallback(const NxFr::Delegate<void(NxFr::StringView)>& Callback);
		NX_ENGINE_API void UnregisterCallback();

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnEnable() override;
		NX_ENGINE_API void OnGui(float TimeStep) override;

	private:
		NxFr::String Input;
		NxFr::Delegate<void(NxFr::StringView)> Callback;
		GUI::Style Style;
	};
}
