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

	System* Application::GetSystem(NxFr::StringId Id)
	{
		return Instance->GetSystems().GetSystem(Id);
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
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Console Arguments", &NxFr::Arguments::Log);
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Setup Project", [&]()
		{
			NEXUS_LOG(Info, Default, "Application %s starting in %s Mode", ProjectInfo.GetName().C(), NxEn::Enum::ToString(ProjectInfo.GetTarget()));
			NxFr::Platform::GetInstance()->SetWorkingDirectory(ProjectInfo.GetRootPath());
		});
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Setup Paths & Folders", &NxFr::Paths::SetupPathsAndFolders);
	}

	void Application::OnShutdown()
	{
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Application duration", []()
		{
			NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		});
		Bootstrap.AppendStep(Bootstrapper::StepBucket::AfterSystem, "Cleanup Folders", &NxFr::Paths::CleanupFolders);
	}

	void Application::OnExecute()
	{
		Ticks.AppendTickOnceCallback(Ticker::TickBucket::Input, "Start TimeManager", { &Time, &TimeManager::Run });
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
