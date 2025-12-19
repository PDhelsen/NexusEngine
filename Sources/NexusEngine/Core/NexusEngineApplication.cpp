#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	const static Command CmdQuit = Command::Create("Application.Quit"_Sid, "Request application to quit", NxFr::Delegate<void()>([]()
	{
		Application::GetInstance()->Quit();
	}));

	const static GUI::Menu::Item MenuItemQuit = GUI::Menu::Item::Create("File/Quit", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("Application.Quit");
	}), 2);

	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication(const NxEn::Project& ProjectInfo)
		: Application(ProjectInfo), InputSchema(), Headless(NxFr::Arguments::Has("Headless"))
	{
		SystemManager& Systems = GetSystems();

		Systems.CreateSystem<SettingsSystem>();
		Systems.CreateSystem<DebugSystem>();
		Systems.CreateSystem<MemorySystem>();
		Systems.CreateSystem<CommandsSystem>();
		Systems.CreateSystem<InputSystem>();
		Systems.CreateSystem<ResourcesSystem>();
		Systems.CreateSystem<AssetsSystem>();
		Systems.CreateSystem<WorldSystem>();
		if (!IsHeadless())
		{
			Systems.CreateSystem<WindowSystem>();
			Systems.CreateSystem<GUISystem>();
		}

		Systems.GetSystem<InputSystem>()->AddSchema("Application"_Sid, &InputSchema);
		if (!IsHeadless())
		{
			WindowSystem* Window = Systems.GetSystem<WindowSystem>();
			Window->GetOnClose() += { this, &Application::Quit };
			Window->SetWindowTitle(GetProject().GetName());
		}
	}

	NexusEngineApplication::~NexusEngineApplication()
	{
		SystemManager& Systems = GetSystems();

		Systems.DestroySystem<SettingsSystem>();
		Systems.DestroySystem<DebugSystem>();
		Systems.DestroySystem<MemorySystem>();
		Systems.DestroySystem<CommandsSystem>();
		Systems.DestroySystem<InputSystem>();
		Systems.DestroySystem<ResourcesSystem>();
		Systems.DestroySystem<AssetsSystem>();
		Systems.DestroySystem<WorldSystem>();
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

		Bootstrap.AppendSystem<SettingsSystem>();
		Bootstrap.AppendSystem<DebugSystem>().AppendDependency<DebugSystem, SettingsSystem>();
		Bootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, DebugSystem>();
		Bootstrap.AppendSystem<CommandsSystem>().AppendDependency<CommandsSystem, MemorySystem>();
		Bootstrap.AppendSystem<InputSystem>();
		Bootstrap.AppendSystem<ResourcesSystem>().AppendDependency<ResourcesSystem, DebugSystem>().AppendDependency<ResourcesSystem, MemorySystem>();
		Bootstrap.AppendSystem<AssetsSystem>().AppendDependency<AssetsSystem, DebugSystem>().AppendDependency<AssetsSystem, MemorySystem>();
		Bootstrap.AppendSystem<WorldSystem>().AppendDependency<WorldSystem, DebugSystem>().AppendDependency<WorldSystem, MemorySystem>();
		if (!IsHeadless())
		{
			Bootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, SettingsSystem>().AppendDependency<InputSystem, WindowSystem>();
			Bootstrap.AppendSystem<GUISystem>().AppendDependency<GUISystem, WindowSystem>().AppendDependency<GUISystem, DebugSystem>();
		}

		Bootstrap.AppendStep(Bootstrapper::StepBucket::AfterSystem, "Apply Settings", []()
		{
			Application::GetSystem<SettingsSystem>()->ApplySettings();
		});
		Bootstrap.AppendStep(Bootstrapper::StepBucket::AfterSystem, "Start Profiler", []()
		{
			Application::GetSystem<DebugSystem>()->AutoStart();
		});
	}

	void NexusEngineApplication::OnShutdown()
	{
		Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendSystem<SettingsSystem>();
		Unbootstrap.AppendSystem<CommandsSystem>();
		Unbootstrap.AppendSystem<ResourcesSystem>();
		Unbootstrap.AppendSystem<AssetsSystem>();
		Unbootstrap.AppendSystem<WorldSystem>();
		Unbootstrap.AppendSystem<DebugSystem>();
		Unbootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, CommandsSystem>().AppendDependency<MemorySystem, ResourcesSystem>().AppendDependency<MemorySystem, AssetsSystem>().AppendDependency<MemorySystem, WorldSystem>();
		Unbootstrap.AppendSystem<InputSystem>();
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
		Ticks.AppendSystem<ResourcesSystem>(Ticker::TickBucket::Engine);
		Ticks.AppendSystem<AssetsSystem>(Ticker::TickBucket::Engine);
		Ticks.AppendSystem<WorldSystem>(Ticker::TickBucket::Engine);
		if (!IsHeadless())
		{
			Ticks.AppendSystem<GUISystem>(Ticker::TickBucket::Output);
			Ticks.AppendSystem<WindowSystem>(Ticker::TickBucket::Output).AppendDependency<WindowSystem, GUISystem>();
		}

		Ticks.AppendTickOnceCallback(NxEn::Ticker::TickBucket::Input, "Parse Commands", { this, &NexusEngineApplication::ParseCommands });
		Ticks.AppendTickCallback(NxEn::Ticker::TickBucket::Project, "World Test", &WorldTest);
	}

	void NexusEngineApplication::ParseCommands()
	{
		NxFr::StringView CommandsList = NxFr::Arguments::Get("Commands");
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
