#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxEn
{
	class NX_ENGINE_API CommandsSystem : public System
	{
	public:
		NX_OBJECT(CommandsSystem)

		static Command* GetCommand(NxFr::StringId Id);
		static void RegisterCommand(Command* Instance);
		static void UnregisterCommand(Command* Instance);

		CommandsSystem();
		~CommandsSystem();

		void Run(NxFr::StringView Cmd);
		void Execute(NxFr::StringView Cmd);
		void File(NxFr::StringView Path);
		void Help();

		uint64 GetQueuedCommandCount() const { return Queue.GetCount(); }
		bool IsExecutingCommand() const { return Current != nullptr; }
		NxFr::StringId GetCurrentCommand() const { return IsExecutingCommand() ? Current->Id : NxFr::StringUtility::Id; };

	protected:
		void OnInitialize() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		struct CommandInfo
		{
			NxFr::StringId Id;
			NxFr::String Args;
			float Delay;
		};

		static CommandInfo ParseCommand(NxFr::StringView Cmd);
		static NxFr::List<NxFr::StringView> ParseCommands(NxFr::StringView Cmds);
		static NxFr::List<NxFr::StringView> ParseArguments(NxFr::StringView Args);

		void PollTerminal();
		void FlushCommands(float TimeStep);
		void Run(const CommandInfo& Info);
		void Execute(const CommandInfo& Info);

		NxFr::Queue<CommandInfo> Queue;
		const CommandInfo* Current;
	};
}
