#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class CommandsSystem;

	class ConsolePanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ConsolePanel)

		NEXUS_ENGINE_API ConsolePanel();
		NEXUS_ENGINE_API ~ConsolePanel();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnDisable() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		void ExecuteCommand();

	private:
		GUI::Menu Menu;

		CommandsSystem* Commands;
		NxFr::Logger* Logger;

		NxFr::List<bool> LoggerFlags;
		NxFr::String Command;
		NxFr::String Search;
	};
}

