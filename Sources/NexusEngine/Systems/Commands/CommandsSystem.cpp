#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/CommandsSystem.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		const static NxFr::StringId Command = "Command"_Sid;
	}
}

namespace NxEn
{
	static Command* CmdHelp = Command::Create("Help"_Sid, "Display avalaible commands", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Help();
	}));

	static Command* CmdFile = Command::Create("Commands.File"_Sid, "Run all commands in the file", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		Application::GetSystem<CommandsSystem>()->File(Path);
	}));

	NxFr::Registry<Command*>& CommandsSystem::GetCommands()
	{
		static NxFr::Registry<Command*> Commands;
		return Commands;
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
		NX_INSTUMENT_FUNCTION();

		Run(ParseCommand(Cmd));
	}

	void CommandsSystem::Execute(NxFr::StringView Cmd)
	{
		NX_INSTUMENT_FUNCTION();

		Execute(ParseCommand(Cmd));
	}

	void CommandsSystem::File(NxFr::StringView Path)
	{
		NX_INSTUMENT_FUNCTION();

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
		NxFr::Registry<Command*>& Commands = GetCommands();

		uint64 Index = 0;
		NxFr::Array<Command*> Cmds = Commands.GetCount();
		for (auto It = Commands.Begin(); It != Commands.End(); ++It)
		{
			Cmds[Index++] = It->Value;
		}
		NxFr::ContainerUtility::Sort<Command*>(Cmds, [](const Command* A, const Command* B)
		{
			return A->GetId().GetString() < B->GetId().GetString();
		});
		for (auto& Cmd : Cmds)
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
			NxFr::List<NxFr::StringView> Cmds = NxFr::StringUtility::SplitAll(Commands, SeparatorCommands);
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
		NX_INSTUMENT_SCOPE(Info.Id);

		NxFr::Registry<Command*>& Commands = GetCommands();
		Command* Instance = Commands.TryGet(Info.Id);
		if (!Instance)
		{
			NX_LOG(Warning, System, "Invalid command: %s", Info.Id.C());
			return;
		}

		Current = &Info;

		NX_LOG(Info, Command, "%s", Info.Request.C());
		Instance->Invoke(Info.Args);

		Current = nullptr;
	}

	CommandsSystem::CommandInfo CommandsSystem::ParseCommand(NxFr::StringView Cmd)
	{
		Cmd = NxFr::StringUtility::TrimLeading(Cmd);
		Cmd = NxFr::StringUtility::TrimTrailing(Cmd);
		NxFr::List<NxFr::StringView> Tokens = NxFr::StringUtility::Tokenize(Cmd, SeparatorArgs);

		NxFr::StringView Id = "";
		NxFr::List<NxFr::StringView> Args;
		NxFr::StringView Delay = "0";

		for (uint64 Index = 0; Index < Tokens.GetCount(); ++Index)
		{
			NxFr::StringView Token = Tokens[Index];
			if (Index == 0)
			{
				Id = Token;
			}
			else if (Token == "-Delay")
			{
				NX_ASSERT(Tokens.IsValidIndex(Index + 1), Default, "Missing delay value");
				Delay = Tokens[Index + 1];
				Index++;
			}
			else
			{
				Args.Append(Token);
			}
		}

		return CommandInfo
		{
			.Request = Cmd,
			.Id = NxFr::StringId(Id),
			.Args = NxFr::Move(Args),
			.Delay = NxFr::StringUtility::FromString<float>(Delay)
		};
	}
}
