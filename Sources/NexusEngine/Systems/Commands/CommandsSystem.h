#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		NEXUS_ENGINE_API extern const NxFr::StringId Command;
	}
}

namespace NxEn
{
	class CommandsSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, CommandsSystem)

		NEXUS_ENGINE_API static Command* GetCommand(NxFr::StringId Id);
		NEXUS_ENGINE_API static void RegisterCommand(Command* Instance);
		NEXUS_ENGINE_API static void UnregisterCommand(Command* Instance);

		NEXUS_ENGINE_API static CommandInfo ParseCommand(NxFr::StringView Cmd);
		NEXUS_ENGINE_API static NxFr::List<CommandInfo> ParseCommands(NxFr::StringView Cmds);
		NEXUS_ENGINE_API static NxFr::List<NxFr::StringView> ParseArguments(NxFr::StringView Args);

		NEXUS_ENGINE_API CommandsSystem();
		NEXUS_ENGINE_API ~CommandsSystem();

		NEXUS_ENGINE_API void Run(NxFr::StringView Cmd);
		NEXUS_ENGINE_API void Run(const CommandInfo& Info);
		NEXUS_ENGINE_API void Execute(NxFr::StringView Cmd);
		NEXUS_ENGINE_API void Execute(const CommandInfo& Info);
		NEXUS_ENGINE_API void Help();

		NEXUS_ENGINE_API bool IsExecutingCommand() const { return Current != nullptr; }
		NEXUS_ENGINE_API uint64 GetQueuedCommandCount() const { return Queue->GetCount(); }
		NEXUS_ENGINE_API const CommandInfo& GetCurrentCommand() const { return IsExecutingCommand() ? *Current : CommandInfo::Dummy; };

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NEXUS_ENGINE_API void PollTerminal();
		NEXUS_ENGINE_API void FlushCommands(float TimeStep);

	private:
		NxFr::Queue<CommandInfo>* Queue;
		const CommandInfo* Current;
		Allocator* Alloc;
	};
}
