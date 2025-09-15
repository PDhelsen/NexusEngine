#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class SettingsPanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, SettingsPanel)

		inline static float Indent = 100.0f;

		NEXUS_ENGINE_API SettingsPanel();
		NEXUS_ENGINE_API ~SettingsPanel();

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnDisable() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		void Fetch();
		void Load();
		void Save();

	private:
		GUI::Menu Menu;
		uint64 Page;

		NxFr::Array<NxFr::String> Pages;
		NxFr::Array<NxFr::Array<NxFr::String>> Names;
		NxFr::Array<NxFr::Array<Setting*>> Values;
	};
}
