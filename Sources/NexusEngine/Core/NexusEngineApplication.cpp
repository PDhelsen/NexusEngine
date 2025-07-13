#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEngine/External/Glfw.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	const static Command CmdQuit = Command("Quit"_Sid, NxFr::Delegate<void()>(Application::GetInstance(), &Application::Quit));

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
		if (!IsHeadless()) Bootstrap.AppendSystem<WindowSystem>().AppendDependency<InputSystem, WindowSystem>();
		Bootstrap.AppendSystem<CommandsSystem>();
	}

	void NexusEngineApplication::OnShutdown()
	{
		Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<DebugSystem>();
		Unbootstrap.AppendSystem<MemorySystem>();
		Unbootstrap.AppendSystem<InputSystem>();
		if (!IsHeadless()) Unbootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, InputSystem>();
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
		Ticks.AppendSystem<CommandsSystem>(Ticker::TickBucket::Input, 10.0f).AppendDependency<CommandsSystem, InputSystem>();
		if (!IsHeadless()) Ticks.AppendSystem<WindowSystem>(Ticker::TickBucket::Output);
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
		NxFr::List<NxFr::StringView> Commands = CommandsList.SplitAll(";");
		for (auto Cmd : Commands)
		{
			NxFr::List<NxFr::StringView> Parts = Cmd.SplitAll(":");
			NxFr::StringId Id = NxFr::StringId(Parts[0]);
			float Delay = Parts.GetCount() > 1 ? (float)NxFr::StringUtility::ToDouble(Parts[1]) : 0.0f;

			CmdSystem->Run(Id, Delay);
		}
	}
}
