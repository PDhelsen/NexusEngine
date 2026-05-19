#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		NX_ENGINE_API extern const NxFr::StringId Command;
	}
}

namespace NxEn
{
	class CommandsSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, CommandsSystem)

		NX_ENGINE_API static Command* GetCommand(NxFr::StringId Id);
		NX_ENGINE_API static void RegisterCommand(Command* Instance);
		NX_ENGINE_API static void UnregisterCommand(Command* Instance);

		NX_ENGINE_API static CommandInfo ParseCommand(NxFr::StringView Cmd);
		NX_ENGINE_API static NxFr::List<CommandInfo> ParseCommands(NxFr::StringView Cmds);
		NX_ENGINE_API static NxFr::List<NxFr::StringView> ParseArguments(NxFr::StringView Args);

		NX_ENGINE_API CommandsSystem();
		NX_ENGINE_API ~CommandsSystem();

		NX_ENGINE_API void Run(NxFr::StringView Cmd);
		NX_ENGINE_API void Run(const CommandInfo& Info);
		NX_ENGINE_API void Execute(NxFr::StringView Cmd);
		NX_ENGINE_API void Execute(const CommandInfo& Info);
		NX_ENGINE_API void File(NxFr::String Path);
		NX_ENGINE_API void Help();

		NX_ENGINE_API bool IsExecutingCommand() const { return Current != nullptr; }
		NX_ENGINE_API uint64 GetQueuedCommandCount() const { return Queue.GetCount(); }
		NX_ENGINE_API const CommandInfo& GetCurrentCommand() const { return IsExecutingCommand() ? *Current : CommandInfo::Dummy; };

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NX_ENGINE_API void PollTerminal();
		NX_ENGINE_API void FlushCommands(float TimeStep);

	private:
		NxFr::Queue<CommandInfo> Queue;
		const CommandInfo* Current;
	};
}
