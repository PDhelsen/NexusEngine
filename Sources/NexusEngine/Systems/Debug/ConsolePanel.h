#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class CommandsSystem;

	class ConsolePanel : public GUI::Panel
	{
		struct Log
		{
			NxFr::String Text;
			bool& Verbosity;
			bool& Channel;
			const GUI::Style& Style;
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ConsolePanel)

		NEXUS_ENGINE_API static const GUI::Style& GetStyle(NxFr::LoggerVerbosity Verbosity);

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

		NxFr::List<Log> Logs;
		NxFr::Dictionary<NxFr::LoggerVerbosity, bool> FlagsVerbosity;
		NxFr::Dictionary<NxFr::StringId, bool> FlagsChannels;
		NxFr::String Command;
		NxFr::String Search;
	};
}

