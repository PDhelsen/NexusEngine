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
		NEXUS_ASSERT(Instance == nullptr, Application, "Application was already created");
		Instance = this;
	}

	Application::~Application()
	{
		Instance = nullptr;
	}

	void Application::Quit()
	{
		NEXUS_LOG(Info, Application, "Application was requested to quit");

		WantsToQuit = true;
	}

	void Application::Restart()
	{
		NEXUS_LOG(Info, Application, "Application was requested to restart");

		EntryPoint::ScheduleRestart();
		Quit();
	}

	void Application::Crash(CrashCode ErrorCode)
	{
		NEXUS_LOG(Info, Application, "Application crashed with code %i", ErrorCode);

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
			NEXUS_LOG(Info, Default, "Application %s starting in %s Mode", ProjectInfo.GetName().C(), NxEn::Enum::ToString(ProjectInfo.GetTarget()));
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
		Time.Run();
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
			float DeltaTime = Time.GetDeltaTime();

			{
				NEXUS_PROFILE_SCOPE("Frame");
				NEXUS_STAT_DECIMAL(NxFr::StatsHeader::FpsId, 1.0f / DeltaTime);
				NEXUS_STAT_DECIMAL(NxFr::StatsHeader::TimerMainId, DeltaTime);

				Ticks.Tick(DeltaTime);
			}

			Time.Tick();
		}
	}
}
