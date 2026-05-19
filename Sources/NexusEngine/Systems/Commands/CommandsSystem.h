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
	class NX_ENGINE_API CommandsSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(CommandsSystem)

		static Command* GetCommand(NxFr::StringId Id);
		static void RegisterCommand(Command* Instance);
		static void UnregisterCommand(Command* Instance);

		static CommandInfo ParseCommand(NxFr::StringView Cmd);
		static NxFr::List<CommandInfo> ParseCommands(NxFr::StringView Cmds);
		static NxFr::List<NxFr::StringView> ParseArguments(NxFr::StringView Args);

		CommandsSystem();
		~CommandsSystem();

		void Run(NxFr::StringView Cmd);
		void Run(const CommandInfo& Info);
		void Execute(NxFr::StringView Cmd);
		void Execute(const CommandInfo& Info);
		void File(NxFr::String Path);
		void Help();

		bool IsExecutingCommand() const { return Current != nullptr; }
		uint64 GetQueuedCommandCount() const { return Queue.GetCount(); }
		const CommandInfo& GetCurrentCommand() const { return IsExecutingCommand() ? *Current : CommandInfo::Dummy; };

	protected:
		void OnInitialize() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void PollTerminal();
		void FlushCommands(float TimeStep);

	private:
		NxFr::Queue<CommandInfo> Queue;
		const CommandInfo* Current;
	};
}
