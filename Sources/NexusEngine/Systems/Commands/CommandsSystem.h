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

		inline static const NxFr::String SeparatorCommands = ";";
		inline static const NxFr::String SeparatorArgs = " ";

		static NxFr::Registry<Command*>& GetCommands();

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
			NxFr::String Request;

			NxFr::StringId Id;
			NxFr::List<NxFr::StringView> Args;
			float Delay;
		};

		void PollTerminal();
		void FlushCommands(float TimeStep);
		void Run(const CommandInfo& Info);
		void Execute(const CommandInfo& Info);
		CommandInfo ParseCommand(NxFr::StringView Cmd);

		NxFr::Queue<CommandInfo> Queue;
		const CommandInfo* Current;
	};
}
