#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	class SettingsSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, SettingsSystem)

		NX_ENGINE_API static Setting* GetSetting(NxFr::StringView Id);
		NX_ENGINE_API static void RegisterSetting(Setting* Instance);
		NX_ENGINE_API static void UnregisterSetting(Setting* Instance);

		NX_ENGINE_API void LoadSettings() const;
		NX_ENGINE_API void SaveSettings() const;
		NX_ENGINE_API void ApplySettings();

		NX_ENGINE_API NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> GetAllSettings() const;
		NX_ENGINE_API NxFr::Array<NxFr::Array<Setting*>> GetAllSettingsSorted() const;

		NX_ENGINE_API NxFr::Event<>& GetOnChange() { return OnChange; }

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;

	private:
		NxFr::Event<> OnChange;
	};
}
