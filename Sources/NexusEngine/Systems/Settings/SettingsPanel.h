#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class NX_ENGINE_API SettingsPanel : public GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(SettingsPanel)

		SettingsPanel();
		~SettingsPanel();

	private:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

	private:
		GUI::Menu Menu;
		GUI::Style Style;

		NxFr::Array<NxFr::Array<Setting*>> Settings;
		uint64 Page;
	};
}
