#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	class NX_ENGINE_API SettingsSystem : public System
	{
	public:
		NX_OBJECT(SettingsSystem)

		inline static const NxFr::String Folder = "Settings";
		inline static const NxFr::String Extension = "settings";

		static NxFr::String Key(NxFr::StringView Page, NxFr::StringView Name);
		static Setting* GetSetting(NxFr::StringView Id);
		static void RegisterSetting(Setting* Instance);
		static void UnregisterSetting(Setting* Instance);

		void LoadSettings() const;
		void SaveSettings() const;
		void ApplySettings();

		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> GetAllSettings() const;
		NxFr::Array<NxFr::Array<Setting*>> GetAllSettingsSorted() const;

		NxFr::Event<>& GetOnChange() { return OnChange; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;

	private:
		NxFr::Event<> OnChange;
	};
}
