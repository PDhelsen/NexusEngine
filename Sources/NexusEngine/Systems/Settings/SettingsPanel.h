#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class SettingsPanel : public GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, SettingsPanel)

		NX_ENGINE_API SettingsPanel();
		NX_ENGINE_API ~SettingsPanel();

	private:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnEnable() override;
		NX_ENGINE_API void OnDisable() override;
		NX_ENGINE_API void OnGui(float TimeStep) override;

	private:
		GUI::Menu Menu;
		GUI::Style Style;

		NxFr::Array<NxFr::Array<Setting*>> Settings;
		uint64 Page;
	};
}
