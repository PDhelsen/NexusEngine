#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"
#include "NexusEngine/Systems/GUI/Elements/Menu.h"
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

		NxFr::Array<NxFr::Array<Setting*>> Settings;
		uint64 Page;
	};
}
