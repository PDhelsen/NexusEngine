#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	class NX_ENGINE_API ConsolePanel : public GUI::Panel
	{
	public:
		NX_OBJECT(ConsolePanel)

		ConsolePanel();
		~ConsolePanel();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		struct Log
		{
			NxFr::String Text;
			NxFr::StringId Style;
			bool& Verbosity;
			bool& Channel;
		};

		void AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message);
		void ClearLogs();
		void ExecuteCommand();

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

