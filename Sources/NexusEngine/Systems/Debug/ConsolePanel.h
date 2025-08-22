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
		void AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message);
		void ClearLogs();

	private:
		GUI::Menu Menu;
		NxFr::Array<GUI::Style> Styles;

		CommandsSystem* Commands;
		NxFr::Logger* Logger;

		NxFr::List<NxFr::Tuple<NxFr::String, uint64>> LoggerLines;
		NxFr::List<bool> LoggerFlags;
		NxFr::String Command;
		NxFr::String Search;
	};
}

