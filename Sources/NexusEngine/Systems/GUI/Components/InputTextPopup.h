#pragma once

namespace NxEn
{
	class InputTextPopup : public GUI::Popup
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, InputTextPopup)

		NEXUS_ENGINE_API static InputTextPopup* GetInstance();

		NEXUS_ENGINE_API void RegisterCallback(const NxFr::Delegate<void(NxFr::StringView)>& Callback);
		NEXUS_ENGINE_API void UnregisterCallback();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

	private:
		NxFr::String Input;
		NxFr::Delegate<void(NxFr::StringView)> Callback;
	};
}
