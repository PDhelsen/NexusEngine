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

		static NxFr::Registry<Setting*> Settings;

		static NxFr::String GetKey(NxFr::StringView Page, NxFr::StringView Name);
		static void GetPageAndName(NxFr::StringView Key, NxFr::StringView& Page, NxFr::StringView& Name);

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
