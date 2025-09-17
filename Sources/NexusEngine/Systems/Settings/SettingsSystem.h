#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	class SettingsSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, SettingsSystem)

		NEXUS_ENGINE_API static Setting* GetSetting(NxFr::StringView Id);
		NEXUS_ENGINE_API static void RegisterSetting(Setting* Instance);
		NEXUS_ENGINE_API static void UnregisterSetting(Setting* Instance);

		NEXUS_ENGINE_API void LoadSettings() const;
		NEXUS_ENGINE_API void SaveSettings() const;
		NEXUS_ENGINE_API void ApplySettings();

		NEXUS_ENGINE_API NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> GetAllSettings() const;
		NEXUS_ENGINE_API NxFr::Array<NxFr::Array<Setting*>> GetAllSettingsSorted() const;

		NEXUS_ENGINE_API NxFr::Event<>& GetOnChange() { return OnChange; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;

	private:
		NxFr::Event<> OnChange;
	};
}
