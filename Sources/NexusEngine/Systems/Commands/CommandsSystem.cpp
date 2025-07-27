#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/CommandsSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(CommandsSystem)

	static NxFr::Dictionary<NxFr::StringId, Command*>& GetCommands()
	{
		static NxFr::Dictionary<NxFr::StringId, Command*> Commands;
		return Commands;
	}

	Command* CommandsSystem::GetCommand(NxFr::StringId Id)
	{
		return GetCommands()[Id];
	}

	void CommandsSystem::RegisterCommand(Command* Instance)
	{
		GetCommands().Append(Instance->GetId(), Instance);
	}

	void CommandsSystem::UnregisterCommand(Command* Instance)
	{
		GetCommands().Remove(Instance->GetId());
	}

	CommandInfo CommandsSystem::ParseCommand(NxFr::StringView Cmd)
	{
		NxFr::StringView IdAndDelay = Cmd.Split(",", 0);
		NxFr::StringId Id = NxFr::StringId(IdAndDelay.Split(":", 0));
		float Delay = (float)NxFr::StringUtility::ToDouble(IdAndDelay.Split(":", 1));

		NxFr::StringView Args = Cmd.Find(",");
		if (!Args.IsEmpty())
		{
			Args = Args.ToView(1, Args.GetCount() - 1);
		}

		return CommandInfo
		{
			.Id = Id,
			.Args = NxFr::Move(Args.ToString()),
			.Delay = Delay
		};
	}

	NxFr::List<CommandInfo> CommandsSystem::ParseCommands(NxFr::StringView Cmds)
	{
		NxFr::List<NxFr::StringView> Commands = Cmds.SplitAll(";");
		NxFr::List<CommandInfo> Infos(Commands.GetCount());
		for (auto Cmd : Commands)
		{
			CommandInfo Info = ParseCommand(Cmd);
			Infos.AppendConstruct(Info);

		}
		return Infos;
	}

	NxFr::List<NxFr::StringView> CommandsSystem::ParseArguments(NxFr::StringView Args)
	{
		return Args.SplitAll(",");
	}

	CommandsSystem::CommandsSystem()
		: Queue(nullptr), Current(nullptr), Alloc(nullptr)
	{
	}

	CommandsSystem::~CommandsSystem()
	{
	}

	void CommandsSystem::Run(NxFr::StringView Cmd)
	{
		Run(ParseCommand(Cmd));
	}

	void CommandsSystem::Run(const CommandInfo& Info)
	{
		Queue->AppendConstruct(Info);
	}

	void CommandsSystem::Execute(NxFr::StringView Cmd)
	{
		Execute(ParseCommand(Cmd));
	}

	void CommandsSystem::Execute(const CommandInfo& Info)
	{
		Current = &Info;
		NEXUS_LOG(Info, Default, "Execute command: %s", Info.Id.C());
		GetCommand(Info.Id)->Invoke(Info.Args);
		Current = nullptr;
	}

	void CommandsSystem::OnInitialize()
	{
		System::OnInitialize();

		Alloc = new Allocator(AllocatorType::General);
		Queue = new NxFr::Queue<CommandInfo>(Alloc);
	}

	void CommandsSystem::OnShutdown()
	{
		System::OnShutdown();

		delete Queue;
		delete Alloc;
	}

	void CommandsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		FlushCommands(TimeStep);
	}

	void CommandsSystem::FlushCommands(float TimeStep)
	{
		while (Queue->GetCount() > 0)
		{
			CommandInfo& Info = Queue->Get();

			Info.Delay -= TimeStep;
			if (Info.Delay > 0.0f)
			{
				break;
			}

			Execute(Info);
			Queue->Remove();
		}
	}

	const static Command CmdHelp = Command::Create("Help"_Sid, "Display avalaible commands", NxFr::Delegate<void()>([]()
	{
		NxFr::Dictionary<NxFr::StringId, Command*>& Commands = GetCommands();
		for (auto& [Id, Cmd] : Commands)
		{
			NEXUS_LOG(Info, Default, "Command: %s - %s", Id.C(), Cmd->GetTooltip().C());
		}
	}));
}
