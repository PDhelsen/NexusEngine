#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	class CommandsSystem;

	class NX_ENGINE_API ConsolePanel : public GUI::Panel
	{
		struct Log
		{
			NxFr::String Text;
			NxFr::StringId Style;
			bool& Verbosity;
			bool& Channel;
		};

	public:
		NX_OBJECT_DECLARATION(ConsolePanel)

		static NxFr::StringId GetStyle(NxFr::LoggerVerbosity Verbosity);

		ConsolePanel();
		~ConsolePanel();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

		void ExecuteCommand();
		void AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message);
		void ClearLogs();

	private:
		GUI::Menu Menu;
		GUI::Style Style;

		NxFr::List<Log> Logs;
		NxFr::Dictionary<NxFr::LoggerVerbosity, bool> FlagsVerbosity;
		NxFr::Dictionary<NxFr::StringId, bool> FlagsChannels;
		NxFr::String Command;
		NxFr::String Search;
		bool Scroll;
	};
}

