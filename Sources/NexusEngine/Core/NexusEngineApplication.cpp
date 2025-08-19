#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEngine/External/Glfw.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId FpsId = "FPS"_Sid;
		const NxFr::StringId TimerMainId = "Timer - Main"_Sid;
	}
}

namespace NxEn
{
	const static Command CmdQuit = Command::Create("Application.Quit"_Sid, "Request application to quit", NxFr::Delegate<void()>([]()
	{
		Application::GetInstance()->Quit();
	}));

	const static GUI::Menu::Item MenuItemQuit = GUI::Menu::Item::Create("File/Quit", NxFr::Delegate<void()>([]()
	{
		Application::GetInstance()->GetSystem<CommandsSystem>()->Execute("Application.Quit");
	}), "", 2);

	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication(const NxEn::Project& ProjectInfo)
		: Application(ProjectInfo), InputSchema(), Headless(NxFr::Arguments::HasFlag("Headless"))
	{
		SystemManager& Systems = GetSystems();

		Systems.CreateSystem<DebugSystem>();
		Systems.CreateSystem<MemorySystem>();
		InputSystem* Inputs = Systems.CreateSystem<InputSystem>();
		Inputs->AddSchema("Application"_Sid, &InputSchema);
		if (!IsHeadless())
		{
			WindowSystem* Window = Systems.CreateSystem<WindowSystem>();
			Window->GetOnClose() += NxFr::Delegate<void()>(this, &Application::Quit);

			Systems.CreateSystem<GUISystem>();
		}
		Systems.CreateSystem<CommandsSystem>();
	}

	void NexusEngineApplication::OnInitialize()
	{
		Application::OnInitialize();
		Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<DebugSystem>();
		Bootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, DebugSystem>();
		Bootstrap.AppendSystem<InputSystem>();
		if (!IsHeadless())
		{
			Bootstrap.AppendSystem<WindowSystem>().AppendDependency<InputSystem, WindowSystem>();
			Bootstrap.AppendSystem<GUISystem>().AppendDependency<GUISystem, WindowSystem>();
		}
		Bootstrap.AppendSystem<CommandsSystem>().AppendDependency<CommandsSystem, MemorySystem>();
	}

	void NexusEngineApplication::OnShutdown()
	{
		Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<DebugSystem>();
		Unbootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, CommandsSystem>();
		Unbootstrap.AppendSystem<InputSystem>();
		if (!IsHeadless())
		{
			Unbootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, InputSystem>();
			Unbootstrap.AppendSystem<GUISystem>().AppendDependency<WindowSystem, GUISystem>();
		}
		Unbootstrap.AppendSystem<CommandsSystem>();

		Unbootstrap.AppendStep([]()
		{
			NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		}, "Application duration");

		StopDebugTools();

		Application::OnShutdown();
	}

	void NexusEngineApplication::OnExecute()
	{
		Application::OnExecute();
		Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<InputSystem>(Ticker::TickBucket::Input);
		Ticks.AppendSystem<CommandsSystem>(Ticker::TickBucket::Input, Ticker::LowFrequency).AppendDependency<CommandsSystem, InputSystem>();
		if (!IsHeadless())
		{
			Ticks.AppendSystem<GUISystem>(Ticker::TickBucket::Output);
			Ticks.AppendSystem<WindowSystem>(Ticker::TickBucket::Output).AppendDependency<WindowSystem, GUISystem>();
		}
		Ticks.AppendSystem<MemorySystem>(Ticker::TickBucket::Cleanup);
		Ticks.AppendSystem<DebugSystem>(Ticker::TickBucket::Cleanup).AppendDependency<DebugSystem, MemorySystem>();

		Ticks.AppendTickOnceCallback({ this, &NexusEngineApplication::ParseCommands }, NxEn::Ticker::TickBucket::Input, "Parse Commands");

		StartDebugTools();
	}

	void NexusEngineApplication::ParseCommands()
	{
		NxFr::StringView CommandsList = NxFr::Arguments::GetValue("Commands");
		if (CommandsList.IsEmpty())
		{
			return;
		}

		CommandsSystem* CmdSystem = GetSystem<CommandsSystem>();
		NxFr::List<CommandInfo> Commands = CommandsSystem::ParseCommands(CommandsList);

		for (auto Cmd : Commands)
		{
			CmdSystem->Run(Cmd);
		}
	}

	void NexusEngineApplication::StartDebugTools()
	{
		DebugSystem* Debug = GetSystem<DebugSystem>();

		NxFr::Logger* Logger = Debug->GetLogger();
		Logger->AddChannel(NxFr::LoggerChannel::Verbose, false);

		NxFr::Stats* Stats = Debug->GetStats();
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::FpsId, Decimal, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::TimerMainId, Decimal, Set);

		Debug->StartTools();
	}

	void NexusEngineApplication::StopDebugTools()
	{
		DebugSystem* Debug = GetSystem<DebugSystem>();
		Debug->StopTools();
	}
}
