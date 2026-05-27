#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

#include "NexusEngine/Application/Flow/EntryPoint.h"

namespace NxEn
{
	Application* Application::Instance = nullptr;

	Application::Application(const Project& ProjectInfo)
		: ProjectInfo(ProjectInfo), Systems(), Bootstrap(), Ticks(), Time(), WantsToQuit(false), CrashReason(0)
	{
		NX_ASSERT(Instance == nullptr, Application, "Application was already created");
		Instance = this;

		NxFr::Globals::Debug::Logs->AddChannel(NxFr::LoggerChannel::Application, true);
	}

	Application::~Application()
	{
		Instance = nullptr;
	}

	void Application::Quit()
	{
		NX_LOG(Info, Application, "Application was requested to quit");

		WantsToQuit = true;
	}

	void Application::Crash(int32 ErrorCode)
	{
		if (CrashReason != 0)
		{
			return;
		}

		NX_LOG(Info, Application, "Application crashed with code %d", ErrorCode);

		CrashReason = ErrorCode;
		EntryPoint::SetErrorCode((int32)ErrorCode);
		WantsToQuit = true;
	}

	void Application::Restart()
	{
		NX_LOG(Info, Application, "Application was requested to restart");

		EntryPoint::ScheduleRestart();
		WantsToQuit = true;
	}

	bool Application::IsRunning() const
	{
		return !WantsToQuit;
	}

	void Application::OnInitialize()
	{
		Bootstrap.AppendStep(Bootstrapper::BootBucket::BeforeSystem, "Console Arguments", []() { NxFr::Globals::Args->Print(); });
		Bootstrap.AppendStep(Bootstrapper::BootBucket::BeforeSystem, "Setup Project", [&]()
		{
			NX_LOG(Info, Application, "Application %s starting in %s Mode", ProjectInfo.GetName().C(), NxFr::StringUtility::ToString(ProjectInfo.GetMode()).C());
			NxFr::Globals::PlatformTarget->SetWorkingDirectory(ProjectInfo.GetRootPath());
		});
		Bootstrap.AppendStep(Bootstrapper::BootBucket::BeforeSystem, "Setup Paths & Folders", &NxFr::Globals::CreatePathsAndFolders);
	}

	void Application::OnShutdown()
	{
		Bootstrap.AppendStep(Bootstrapper::BootBucket::BeforeSystem, "Application duration", []()
		{
			NX_LOG(Info, Application, "Application last for %llu seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		});
		Bootstrap.AppendStep(Bootstrapper::BootBucket::AfterSystem, "Cleanup Folders", &NxFr::Globals::DestroyTempFolder);
	}

	void Application::OnRun()
	{
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

	void Application::Run()
	{
		OnRun();
		Ticks.Run();
		Time.Run();

		while (IsRunning())
		{
			NX_INSTUMENT_SCOPE("Frame");

			float DeltaTime = Time.GetDeltaTime();
			Ticks.Tick(DeltaTime);
			Time.Tick();
		}
	}
}
