#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Elements/Popup.h"

namespace NxEn
{
	class NX_ENGINE_API InputTextPopup : public GUI::Popup
	{
	public:
		NX_OBJECT(InputTextPopup)

		void RegisterCallback(const NxFr::Delegate<void(NxFr::StringView)>& Callback);
		void UnregisterCallback();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		NxFr::String Input;
		NxFr::Delegate<void(NxFr::StringView)> Callback;
	};
}
