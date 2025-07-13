#pragma once

#include "NexusEngine/Systems/System.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxEn
{
	class CommandsSystem : public System
	{
	private:
		struct CommandInfo
		{
			NxFr::StringId Id;
			float Delay;
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, CommandsSystem)

		NEXUS_ENGINE_API static Command* GetCommand(NxFr::StringId Id);
		NEXUS_ENGINE_API static void RegisterCommand(Command* Instance);
		NEXUS_ENGINE_API static void UnregisterCommand(Command* Instance);

		NEXUS_ENGINE_API CommandsSystem();
		NEXUS_ENGINE_API ~CommandsSystem();

		NEXUS_ENGINE_API void Run(NxFr::StringId Id, float Delay = 0.0f);
		NEXUS_ENGINE_API void Execute(NxFr::StringId Id);

		NEXUS_ENGINE_API bool IsExecutingCommand() const;
		NEXUS_ENGINE_API NxFr::StringId GetCurrentCommand() const;

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API void FlushCommands(float TimeStep);

	private:
		NxFr::Queue<CommandInfo> Queue;
		Command* Current;
	};
}
