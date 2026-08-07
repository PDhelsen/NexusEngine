#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEn
{
	static Command* CmdQuit = Command::Create("Application.Quit"_Sid, "Request application to quit", NxFr::Delegate<void()>([]()
	{
		Application::GetInstance()->Quit();
	}));

	static const NxEn::GUI::Menu::Item* MenuItemQuit = GUI::Menu::Create("File/Quit", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("Application.Quit");
	}), 2);

	NX_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication(const NxEn::Project& ProjectInfo)
		: Application(ProjectInfo), Inputs(nullptr), Headless(NxFr::Globals::Args->Has("Headless"))
	{
		SystemManager& Systems = GetSystems();
		Systems.CreateSystem<DebugSystem>();
		Systems.CreateSystem<MemorySystem>();
		Systems.CreateSystem<JobSystem>();
		Systems.CreateSystem<CommandsSystem>();
		Systems.CreateSystem<SettingsSystem>();
		Systems.CreateSystem<InputSystem>();
		Systems.CreateSystem<WindowSystem>();
		Systems.CreateSystem<GUISystem>();
		Systems.CreateSystem<ResourcesSystem>();
		Systems.CreateSystem<AssetsSystem>();
		Systems.CreateSystem<WorldSystem>();
	}

	NexusEngineApplication::~NexusEngineApplication()
	{
		SystemManager& Systems = GetSystems();
		Systems.DestroySystem<DebugSystem>();
		Systems.DestroySystem<MemorySystem>();
		Systems.DestroySystem<JobSystem>();
		Systems.DestroySystem<CommandsSystem>();
		Systems.DestroySystem<SettingsSystem>();
		Systems.DestroySystem<InputSystem>();
		Systems.DestroySystem<WindowSystem>();
		Systems.DestroySystem<GUISystem>();
		Systems.DestroySystem<ResourcesSystem>();
		Systems.DestroySystem<AssetsSystem>();
		Systems.DestroySystem<WorldSystem>();
	}

	void NexusEngineApplication::OnInitialize()
	{
		Application::OnInitialize();
		Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendStep(Bootstrapper::BootBucket::BeforeSystem, "Connect event HID - Engine", [&]()
		{
			Inputs = new Input::Schema();
			GetSystem<InputSystem>()->AddSchema("Engine"_Sid, Inputs);

			WindowSystem* Window = GetSystem<WindowSystem>();
			Window->GetOnClose() += { (Application*)this, &Application::Quit };
			Window->SetWindowTitle(GetProject().GetName());
		});

		Bootstrap.AppendSystem<DebugSystem>();
		Bootstrap.AppendSystem<CommandsSystem>();
		Bootstrap.AppendSystem<SettingsSystem>();
		Bootstrap.AppendSystem<JobSystem>().AppendDependency<JobSystem, SettingsSystem>();
		Bootstrap.AppendSystem<MemorySystem>().AppendDependency<MemorySystem, DebugSystem>();
		Bootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, SettingsSystem>();
		Bootstrap.AppendSystem<InputSystem>().AppendDependency<InputSystem, WindowSystem>();
		Bootstrap.AppendSystem<GUISystem>().AppendDependency<GUISystem, WindowSystem>();
		Bootstrap.AppendSystem<ResourcesSystem>().AppendDependency<ResourcesSystem, DebugSystem>();
		Bootstrap.AppendSystem<AssetsSystem>().AppendDependency<AssetsSystem, ResourcesSystem>();
		Bootstrap.AppendSystem<WorldSystem>().AppendDependency<WorldSystem, AssetsSystem>();

		Bootstrap.AppendStep(Bootstrapper::BootBucket::AfterSystem, "Apply Settings", []()
		{
			GetSystem<SettingsSystem>()->ApplySettings();
		});
		Bootstrap.AppendStep(Bootstrapper::BootBucket::AfterSystem, "Start Profiler", []()
		{
			GetSystem<DebugSystem>()->AutoStart();
		});
	}

	void NexusEngineApplication::OnShutdown()
	{
		Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendStep(Bootstrapper::BootBucket::BeforeSystem, "Disconnect event HID - Engine", [&]()
		{
			GetSystem<InputSystem>()->RemoveSchema("Engine"_Sid);
			delete Inputs;

			WindowSystem* Window = GetSystem<WindowSystem>();
			Window->GetOnClose() -= { (Application*)this, & Application::Quit };
		});

		Unbootstrap.AppendSystem<DebugSystem>();
		Unbootstrap.AppendSystem<MemorySystem>();
		Unbootstrap.AppendSystem<JobSystem>();
		Unbootstrap.AppendSystem<CommandsSystem>();
		Unbootstrap.AppendSystem<SettingsSystem>();
		Unbootstrap.AppendSystem<InputSystem>();
		Unbootstrap.AppendSystem<GUISystem>();
		Unbootstrap.AppendSystem<WorldSystem>();
		Unbootstrap.AppendSystem<WindowSystem>().AppendDependency<WindowSystem, InputSystem>().AppendDependency<WindowSystem, GUISystem>();
		Unbootstrap.AppendSystem<AssetsSystem>().AppendDependency<AssetsSystem, WorldSystem>();
		Unbootstrap.AppendSystem<ResourcesSystem>().AppendDependency<ResourcesSystem, AssetsSystem>();

		Application::OnShutdown();
	}

	void NexusEngineApplication::OnRun()
	{
		Application::OnRun();
		Ticker& Ticks = GetTicker();

		Ticks.AppendSystem<InputSystem>(Ticker::TickBucket::Input);
		Ticks.AppendSystem<CommandsSystem>(Ticker::TickBucket::Input, Ticker::LowFrequency).AppendDependency<CommandsSystem, InputSystem>();
		Ticks.AppendSystem<ResourcesSystem>(Ticker::TickBucket::Engine);
		Ticks.AppendSystem<AssetsSystem>(Ticker::TickBucket::Engine).AppendDependency<AssetsSystem, ResourcesSystem>();
		Ticks.AppendSystem<WorldSystem>(Ticker::TickBucket::Engine).AppendDependency<WorldSystem, AssetsSystem>();
		Ticks.AppendSystem<GUISystem>(Ticker::TickBucket::Output);
		Ticks.AppendSystem<WindowSystem>(Ticker::TickBucket::Output).AppendDependency<WindowSystem, GUISystem>();
		Ticks.AppendSystem<MemorySystem>(Ticker::TickBucket::Cleanup);
		Ticks.AppendSystem<DebugSystem>(Ticker::TickBucket::Cleanup).AppendDependency<DebugSystem, MemorySystem>();
	}
}
