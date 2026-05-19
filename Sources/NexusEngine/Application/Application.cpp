#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		const NxFr::StringId Application = "Application"_Sid;
	}
}

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
		NxFr::Globals::Debug::Logs->AddChannel(NxFr::LoggerChannel::Application, true);
		NX_ASSERT(Instance == nullptr, Application, "Application was already created");
		Instance = this;
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

	void Application::Restart()
	{
		NX_LOG(Info, Application, "Application was requested to restart");

		EntryPoint::ScheduleRestart();
		Quit();
	}

	void Application::Crash(CrashCode ErrorCode)
	{
		NX_LOG(Info, Application, "Application crashed with code %d", ErrorCode);

		EntryPoint::SetErrorCode((int8)ErrorCode);
		Quit();
	}

	bool Application::IsRunning() const
	{
		return !WantsToQuit && EntryPoint::GetErrorCode() == 0;
	}

	void Application::OnInitialize()
	{
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Console Arguments", []() { NxFr::Globals::Args->Print(); });
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Setup Project", [&]()
		{
			NX_LOG(Info, Default, "Application %s starting in %s Mode", ProjectInfo.GetName().C(), NxFr::StringUtility::ToString(ProjectInfo.GetTarget()).C());
			NxFr::Globals::PlatformTarget->SetWorkingDirectory(ProjectInfo.GetRootPath());
		});
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Setup Paths & Folders", &NxFr::Globals::CreatePathsAndFolders);
	}

	void Application::OnShutdown()
	{
		Bootstrap.AppendStep(Bootstrapper::StepBucket::BeforeSystem, "Application duration", []()
		{
			NX_LOG(Info, Default, "Application last for %llu seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		});
		Bootstrap.AppendStep(Bootstrapper::StepBucket::AfterSystem, "Cleanup Folders", &NxFr::Globals::DestroyTempFolder);
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
			NX_INSTUMENT_SCOPE("Frame");

			float DeltaTime = Time.GetDeltaTime();
			Ticks.Tick(DeltaTime);
			Time.Tick();
		}
	}
}
