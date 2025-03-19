#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	static Application* Instance = nullptr;

	Application* Application::GetInstance()
	{
		return Instance;
	}

	Application::Application()
		: Systems(), Bootstrap(), Ticks(), Debug(), Time(), WantsToQuit(false)
	{
		NEXUS_ASSERT(Instance == nullptr, Default, "Application was already created");
		Instance = this;
	}

	Application::~Application()
	{
		Instance = nullptr;
	}

	void Application::Quit()
	{
		NEXUS_LOG(Info, Default, "Application was requested to quit");

		WantsToQuit = true;
	}

	void Application::Restart()
	{
		NEXUS_LOG(Info, Default, "Application was requested to restart");

		EntryPoint::ScheduleRestart();
		Quit();
	}

	void Application::Crash(CrashCode ErrorCode)
	{
		NEXUS_LOG(Info, Default, "Application crashed with code %i", ErrorCode);

		EntryPoint::SetErrorCode((int8)ErrorCode);
		Quit();
	}

	bool Application::IsRunning() const
	{
		return !WantsToQuit && EntryPoint::GetErrorCode() == 0;
	}

	void Application::OnInitialize(Bootstrapper& Bootstrap, SystemManager& Systems)
	{
		Bootstrap.AppendStep("Generate Folders", &NxFr::Paths::CreateFrameworkFolders);
		Bootstrap.AppendStep("Initialize Debug Manager", [&]()
		{
			NxFr::Path Folder = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
			bool AutoStart = NxFr::Arguments::HasFlag("Profile", false);
			Debug = new DebugManager(Folder, AutoStart);

			NxFr::Globals::Logs = Debug->GetLogger();
			NxFr::Globals::Statistiques = Debug->GetStats();
			NxFr::Globals::Instrumentor = Debug->GetInstrumentor();

		});
		Bootstrap.AppendStep("Initialize Time Manager", [&]() { Time = new TimeManager(); });
	}

	void Application::OnShutdown(Bootstrapper& Unbootstrap, SystemManager& Systems)
	{
		Unbootstrap.AppendStep("Clear Systems", NxFr::Delegate<void()>(&Systems, &SystemManager::ClearSystems));
		Unbootstrap.AppendStep("Shutdown Time Manager", [&]() { delete Time; Time = nullptr; });
		Unbootstrap.AppendStep("Shutdown Debug Manager", [&]()
		{
			delete Debug;
			Debug = nullptr;

			NxFr::Globals::Logs = nullptr;
			NxFr::Globals::Statistiques = nullptr;
			NxFr::Globals::Instrumentor = nullptr;
		});
	}

	void Application::OnExecute(Ticker& Ticks, SystemManager& Systems)
	{
	}

	void Application::Run()
	{
		Initialize();
		Execute();
		Shutdown();
	}

	void Application::Initialize()
	{
		OnInitialize(Bootstrap, Systems);
		Bootstrap.RunBoot(Systems);
	}

	void Application::Shutdown()
	{
		OnShutdown(Bootstrap, Systems);
		Bootstrap.RunUnboot(Systems);
	}

	void Application::Execute()
	{
		OnExecute(Ticks, Systems);
		Ticks.Run(Systems);

		Time->Run();
		while (IsRunning())
		{
			Ticks.Tick();

			Debug->Flush();
			Time->Tick();
		}
	}
}
