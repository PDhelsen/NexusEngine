#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	class NX_ENGINE_API SettingsPanel : public GUI::Panel
	{
	public:
		NX_OBJECT(SettingsPanel)

		SettingsPanel();
		~SettingsPanel();

	private:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		GUI::Menu Menu;
		GUI::Style Style;

		NxFr::Array<NxFr::Array<Setting*>> Settings;
		uint64 Page;
	};
}
