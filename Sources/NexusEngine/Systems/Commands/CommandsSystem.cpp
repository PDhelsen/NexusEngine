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
	static NxFr::String SplitCommands = ";";
	static NxFr::String SplitArgs = " ";
	static NxFr::String SplitOptions = "-";

	static NxFr::Dictionary<NxFr::StringId, Command*>& GetCommands()
	{
		static NxFr::Dictionary<NxFr::StringId, Command*> Commands;
		return Commands;
	}

	const static Command CmdHelp = Command::Create("Help"_Sid, "Display avalaible commands", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Help();
	}));

	const static Command CmdFile = Command::Create("Commands.File"_Sid, "Run all commands in the file", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		Application::GetSystem<CommandsSystem>()->File(Path);
	}));

	const static Command CmdDummy = Command::Create("Commands.Dummy"_Sid, "Dummy command", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView First, NxFr::StringView Second)
	{
		NX_LOG(Info, Command, "First: %s \nSecond: %s", NxFr::String(First).C(), NxFr::String(Second).C());
	}));

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

	void CommandsSystem::Execute(NxFr::StringView Cmd)
	{
		Execute(ParseCommand(Cmd));
	}

	void CommandsSystem::File(NxFr::StringView Path)
	{
		Path = NxFr::Path::Combine(Application::GetInstance()->GetProject().GetRootPath(), Path);
		if (Path.IsEmpty() || !NxFr::Path::Exist(Path))
		{
			NX_LOG(Warning, System, "Invalid command file path %s", Path.C());
			return;
		}

		NxFr::TextStream File(Path);
		File.Open(NxFr::File::Mode::Read);

		while (!File.IsAtTheEnd())
		{
			Run(File.ReadLine());
		}

		File.Close();
	}

	void CommandsSystem::Help()
	{
		NxFr::Array<Command*> Commands = NxFr::ContainerUtility::ToArrayValues(GetCommands());
		NxFr::ContainerUtility::Sort<Command*>(Commands, [](const Command* A, const Command* B)
		{
			return A->GetId().GetString() < B->GetId().GetString();
		});
		for (auto& Cmd : Commands)
		{
			NX_LOG(Info, Default, "Command: %s - %s", Cmd->GetId().C(), Cmd->GetTooltip().C());
		}
	}

	void CommandsSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::StringView Commands = NxFr::Globals::Args->Get("Commands");
		if (!Commands.IsEmpty())
		{
			NxFr::List<NxFr::StringView> Cmds = ParseCommands(Commands);
			for (auto Cmd : Cmds)
			{
				Run(Cmd);
			}
		}
	}

	void CommandsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		PollTerminal();
		FlushCommands(TimeStep);
	}

	CommandsSystem::CommandInfo CommandsSystem::ParseCommand(NxFr::StringView Cmd)
	{
		Cmd = NxFr::StringUtility::TrimLeading(Cmd);
		Cmd = NxFr::StringUtility::TrimTrailing(Cmd);
		NxFr::List<NxFr::StringView> Parts = NxFr::StringUtility::SplitAll(Cmd, SplitArgs);

		bool Options = false;
		uint64 Index = 0;

		NxFr::StringView Id = Parts[Index++];

		NxFr::String Args;
		while (Index < Parts.GetCount())
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

	NxFr::List<NxFr::StringView> CommandsSystem::ParseCommands(NxFr::StringView Cmds)
	{
		return NxFr::StringUtility::SplitAll(Cmds, SplitCommands);
	}

	NxFr::List<NxFr::StringView> CommandsSystem::ParseArguments(NxFr::StringView Args)
	{
		return NxFr::StringUtility::SplitAll(Args, SplitArgs);
	}

	void CommandsSystem::PollTerminal()
	{
		NxFr::String Request = NxFr::Globals::PlatformTarget->ReadFromTerminal();
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

	void CommandsSystem::Run(const CommandInfo& Info)
	{
		Queue.AppendConstruct(Info);
	}

	void CommandsSystem::Execute(const CommandInfo& Info)
	{
		NX_INSTUMENT_SCOPE(Info.Id.C());

		Command** Instance = GetCommands().TryGet(Info.Id);
		if (!Instance)
		{
			NX_LOG(Warning, System, "Invalid command: %s", Info.Id.C());
			return;
		}

		Current = &Info;

		NX_LOG(Info, Command, "%s %s", Info.Id.C(), Info.Args.C());
		(*Instance)->Invoke(ParseArguments(Info.Args));

		Current = nullptr;
	}
}
