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
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		void ExecuteCommand();

	private:
		CommandsSystem* Commands;
		NxFr::Logger* Logger;

		NxFr::String Cmd;
	};
}

