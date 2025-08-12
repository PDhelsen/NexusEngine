#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	static Application* Instance = nullptr;

	Application* Application::GetInstance()
	{
		return Instance;
	}

	Application::Application(const Project& ProjectInfo)
		: ProjectInfo(ProjectInfo), Bootstrap(), Ticks(), Systems(), Time(), WantsToQuit(false)
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

	void Application::OnInitialize()
	{
		Bootstrap.AppendStep(&NxFr::Arguments::Log, "Console Arguments");
		Bootstrap.AppendStep([&]()
		{
			NEXUS_LOG(Info, Default, "Application starting in %s Mode", NxEn::Enum::ProjectModeToString(ProjectInfo.GetTarget()));
			NxFr::Platform::GetInstance()->SetWorkingDirectory(ProjectInfo.GetRootPath());
		}, "Setup Project");
		Bootstrap.AppendStep(&NxFr::Paths::SetupPathsAndFolders, "Setup Paths & Folders");
		Bootstrap.AppendStep([&]()
			{
				Systems.GetOnSystemChanged() += { &Ticks, & Ticker::PatchSystem };
			}, "Connect Ticker to SystemManager");
	}

	void Application::OnShutdown()
	{
		Bootstrap.AppendStep([&]()
			{
				Systems.GetOnSystemChanged() -= { &Ticks, & Ticker::PatchSystem };
			}, "Disconnect Ticker from SystemManager");
		Bootstrap.AppendStep(NxFr::Delegate<void()>(&Systems, &SystemManager::ClearSystems), "Clear Systems");
		Bootstrap.AppendStep(&NxFr::Paths::CleanupFolders, "Cleanup Folders");
	}

	void Application::OnExecute()
	{
		Ticks.AppendTickOnceCallback({ &Time, &TimeManager::Run }, Ticker::TickBucket::Input, "Start Ticking");
	}

	void Application::Run()
	{
		Initialize();
		Execute();
		Shutdown();
	}

	void Application::Initialize()
	{
		OnInitialize();
		Bootstrap.RunBoot();
	}

	void Application::Shutdown()
	{
		OnShutdown();
		Bootstrap.RunUnboot();
	}

	void Application::Execute()
	{
		OnExecute();
		Ticks.Run();

		while (IsRunning())
		{
			NEXUS_PROFILE_SCOPE("Frame");

			float DeltaTime = Time.GetDeltaTime();
			NEXUS_STAT_DECIMAL(StatsHeader::FpsId, 1.0f / DeltaTime);

			Ticks.Tick(DeltaTime);
			Time.Tick();
		}
	}
}
