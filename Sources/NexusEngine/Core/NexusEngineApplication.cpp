#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEngine/External/Glfw.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication(const NxEn::Project& ProjectInfo)
		: Application(ProjectInfo), Headless(NxFr::Arguments::HasFlag("Headless"))
	{
		SystemManager& Systems = GetSystems();

		Systems.CreateSystem<DebugSystem>();
		Systems.CreateSystem<MemorySystem>();
		Systems.CreateSystem<InputSystem>();
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

		Bootstrap.AppendStep(Glfw::Initialize, "Glfw - Initialize");

		Bootstrap.AppendSystem<MemorySystem>();
		Bootstrap.AppendSystem<DebugSystem>();
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
		Unbootstrap.AppendStep(Glfw::Shutdown, "Glfw - Shutdown");

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

		ParseCommands();
	}

	void NexusEngineApplication::ParseCommands()
	{
		NxFr::StringView CommandsList = NxFr::Arguments::GetValue("Commands");
		if (CommandsList.IsEmpty())
		{
			return;
		}

		CommandsSystem* CmdSystem = GetSystems().GetSystem<CommandsSystem>();
		NxFr::List<CommandInfo> Commands = CommandsSystem::ParseCommands(CommandsList);

		for (auto Cmd : Commands)
		{
			CmdSystem->Run(Cmd);
		}
	}

	const static Command CmdQuit = Command::Create("Quit"_Sid, "Request application to quit", NxFr::Delegate<void()>([]() {Application::GetInstance()->Quit(); }));
}
