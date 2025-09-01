#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEngine/External/Glfw.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		const NxFr::StringId Application = "Application"_Sid;
		const NxFr::StringId System = "System"_Sid;
	}

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
		Application::GetSystem<CommandsSystem>()->Execute("Application.Quit");
	}), "", 2);

	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication(const NxEn::Project& ProjectInfo)
		: Application(ProjectInfo), InputSchema(), Headless(NxFr::Arguments::HasFlag("Headless"))
	{
		SystemManager& Systems = GetSystems();

		Systems.CreateSystem<DebugSystem>();
		Systems.CreateSystem<MemorySystem>();
		Systems.CreateSystem<InputSystem>();
		Systems.CreateSystem<CommandsSystem>();
		if (!IsHeadless())
		{
			Systems.CreateSystem<WindowSystem>();
			Systems.CreateSystem<GUISystem>();
		}

		NxFr::Logger* Logger = Systems.GetSystem<DebugSystem>()->GetLogger();
		Logger->AddChannel(NxFr::LoggerChannel::Application, true);
		Logger->AddChannel(NxFr::LoggerChannel::System, true);
		Systems.GetSystem<InputSystem>()->AddSchema("Application"_Sid, &InputSchema);
		if (!IsHeadless())
		{
			Systems.GetSystem<WindowSystem>()->GetOnClose() += NxFr::Delegate<void()>(this, &Application::Quit);
		}
	}

	NexusEngineApplication::~NexusEngineApplication()
	{
		SystemManager& Systems = GetSystems();

		Systems.DestroySystem<DebugSystem>();
		Systems.DestroySystem<MemorySystem>();
		Systems.DestroySystem<InputSystem>();
		Systems.DestroySystem<CommandsSystem>();
		if (!IsHeadless())
		{
			Systems.DestroySystem<WindowSystem>();
			Systems.DestroySystem<GUISystem>();
		}
	}

	void NexusEngineApplication::OnInitialize()
	{
		Application::OnInitialize();
		Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendSystem<DebugSystem>();
		Bootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, DebugSystem>();
		Bootstrap.AppendSystem<InputSystem>();
		Bootstrap.AppendSystem<CommandsSystem>().AppendDependency<CommandsSystem, MemorySystem>();
		if (!IsHeadless())
		{
			Bootstrap.AppendSystem<WindowSystem>().AppendDependency<InputSystem, WindowSystem>();
			Bootstrap.AppendSystem<GUISystem>().AppendDependency<GUISystem, WindowSystem>();
		}

		Bootstrap.AppendStep(Bootstrapper::StepBucket::AfterSystem, "Initialize Stats", []()
		{
			NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
			NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::FpsId, Decimal, Set);
			NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::TimerMainId, Decimal, Set);
			Stats->Initialize();
		});
	}

	void NexusEngineApplication::OnShutdown()
	{
		Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<DebugSystem>();
		Unbootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, CommandsSystem>();
		Unbootstrap.AppendSystem<InputSystem>();
		Unbootstrap.AppendSystem<CommandsSystem>();
		if (!IsHeadless())
		{
			Unbootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, InputSystem>();
			Unbootstrap.AppendSystem<GUISystem>().AppendDependency<WindowSystem, GUISystem>();
		}

		Application::OnShutdown();
	}

	void NexusEngineApplication::OnExecute()
	{
		Application::OnExecute();
		Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<InputSystem>(Ticker::TickBucket::Input);
		Ticks.AppendSystem<CommandsSystem>(Ticker::TickBucket::Input, Ticker::LowFrequency).AppendDependency<CommandsSystem, InputSystem>();
		Ticks.AppendSystem<MemorySystem>(Ticker::TickBucket::Cleanup);
		Ticks.AppendSystem<DebugSystem>(Ticker::TickBucket::Cleanup).AppendDependency<DebugSystem, MemorySystem>();
		if (!IsHeadless())
		{
			Ticks.AppendSystem<GUISystem>(Ticker::TickBucket::Output);
			Ticks.AppendSystem<WindowSystem>(Ticker::TickBucket::Output).AppendDependency<WindowSystem, GUISystem>();
		}

		Ticks.AppendTickOnceCallback(NxEn::Ticker::TickBucket::Input, "Parse Commands", { this, &NexusEngineApplication::ParseCommands });
		Ticks.AppendTickOnceCallback(NxEn::Ticker::TickBucket::Input, "Auto start debug system", []()
		{
				Application::GetSystem<DebugSystem>()->AutoStart();
		});
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
}
