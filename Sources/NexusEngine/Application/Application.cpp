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

	Application::Application()
		: Systems(), Bootstrap(), Ticks(), Time(nullptr), WantsToQuit(false)
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
		Bootstrap.AppendStep("Initialize Time Manager", [&]() { Time = new TimeManager(); });
	}

	void Application::OnShutdown(Bootstrapper& Unbootstrap, SystemManager& Systems)
	{
		Unbootstrap.AppendStep("Shutdown Time Manager", [&]() { delete Time; Time = nullptr; });
		Unbootstrap.AppendStep("Clear Systems", NxFr::Delegate<void()>(&Systems, &SystemManager::ClearSystems));
	}

	void Application::OnExecute(Ticker& Ticks, SystemManager& Systems)
	{
		Ticks.AppendTickOnceCallback([&]() { Time->Run(); }, Ticker::TickBucket::Input);
		Ticks.AppendTickCallback([&]() { Time->Tick(); }, Ticker::TickBucket::Cleanup, "Time Manager");
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

		while (IsRunning())
		{
			NEXUS_PROFILE_SCOPE("Frame");

			float DeltaTime = Time->GetDeltaTime();
			Ticks.Tick(DeltaTime);
		}
	}
}
