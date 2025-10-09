#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/CommandsSystem.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		const NxFr::StringId Command = "Command"_Sid;
	}
}

namespace NxEn
{
	static const char* SplitCommands = ";";
	static const char* SplitArgs = " ";
	static const char* SplitOptions = "-";

	static NxFr::Dictionary<NxFr::StringId, Command*>& GetCommands()
	{
		static NxFr::Dictionary<NxFr::StringId, Command*> Commands;
		return Commands;
	}

	const static Command CmdHelp = Command::Create("Help"_Sid, "Display avalaible commands", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Help();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(CommandsSystem)

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
		Cmd = NxFr::StringUtility::TrimLeading(Cmd);
		Cmd = NxFr::StringUtility::TrimTrailing(Cmd);
		NxFr::List<NxFr::StringView> Parts = NxFr::StringUtility::SplitAll(Cmd, SplitArgs);

		bool Options = false;
		uint64 Index = 0;

		NxFr::StringView Id = Parts[Index++];

		NxFr::String Args;
		while(Index < Parts.GetCount())
		{
			NxFr::StringView Arg = Parts[Index];
			if (NxFr::StringUtility::Start(Arg, SplitOptions))
			{
				Options = true;
				break;
			}

			if (Index > 1)
			{
				Args += " ";
			}
			Args += Arg;


			Index++;
		}

		NxFr::StringView Delay = "0";
		while (Index < Parts.GetCount())
		{
			if (Parts[Index] == "-Delay" && (Index + 1) < Parts.GetCount())
			{
				Delay = Parts[Index + 1];
				Index += 2;
			}
			else
			{
				Index++;
			}
		}

		return CommandInfo
		{
			.Id = NxFr::StringId(Id),
			.Args = NxFr::Move(Args),
			.Delay = NxFr::StringUtility::FromString<float>(Delay)
		};
	}

	NxFr::List<CommandInfo> CommandsSystem::ParseCommands(NxFr::StringView Cmds)
	{
		NxFr::List<NxFr::StringView> Commands = NxFr::StringUtility::SplitAll(Cmds, SplitCommands);
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
		return NxFr::StringUtility::SplitAll(Args, SplitArgs);
	}

	CommandsSystem::CommandsSystem()
		: Queue(), Current(nullptr)
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
		Queue.AppendConstruct(Info);
	}

	void CommandsSystem::Execute(NxFr::StringView Cmd)
	{
		Execute(ParseCommand(Cmd));
	}

	void CommandsSystem::Execute(const CommandInfo& Info)
	{
		NEXUS_PROFILE_SCOPE(Info.Id.C());

		Current = &Info;

		Command** Target = GetCommands().TryGet(Info.Id);
		bool IsValid = Target != nullptr;

		if (!IsValid)
		{
			NEXUS_LOG(Warning, System, "Invalid command: %s", Info.Id.C());
		}
		else if (!Info.Args.IsEmpty())
		{
			NEXUS_LOG(Info, Command, "%s: %s", Info.Id.C(), Info.Args.C());
		}
		else
		{
			NEXUS_LOG(Info, Command, "%s", Info.Id.C());
		}

		if (IsValid)
		{
			(*Target)->Invoke(Info.Args);
		}
		
		Current = nullptr;
	}

	void CommandsSystem::Help()
	{
		NxFr::Dictionary<NxFr::StringId, Command*>& Commands = GetCommands();
		for (auto& [Id, Cmd] : Commands)
		{
			NEXUS_LOG(Info, Default, "Command: %s - %s", Id.C(), Cmd->GetTooltip().C());
		}
	}

	void CommandsSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->AddChannel(NxFr::LoggerChannel::Command, true);
	}

	void CommandsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		PollTerminal();
		FlushCommands(TimeStep);
	}

	void CommandsSystem::PollTerminal()
	{
		NxFr::String Request = NxFr::Platform::GetInstance()->ReadFromTerminal();
		if (Request.IsEmpty())
		{
			return;
		}

		Run(Request);
	}

	void CommandsSystem::FlushCommands(float TimeStep)
	{
		while (Queue.GetCount() > 0)
		{
			CommandInfo& Info = Queue.Get();

			Info.Delay -= TimeStep;
			if (Info.Delay > 0.0f)
			{
				break;
			}

			Execute(Info);
			Queue.Remove();
		}
	}
}
