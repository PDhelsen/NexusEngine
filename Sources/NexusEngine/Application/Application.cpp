#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

#include "NexusEngine/Misc/DebugManager.h"

namespace NxEn
{
	static Application* Instance = nullptr;

	Application* Application::GetInstance()
	{
		return Instance;
	}

	Application::Application()
		: Systems(), Bootstrap(), Ticks(), WantsToQuit(false)
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

	System* Application::GetSystem(NxFr::StringId Type)
	{
		return Systems[Type];
	}

	void Application::RegisterSystem(NxFr::StringId Type, System* System)
	{
		NEXUS_ASSERT(!Systems.ContainsKey(Type), Default, "System is already created");
		Systems.Append(Type, System);
	}

	void Application::UnregisterSystem(NxFr::StringId Type)
	{
		NEXUS_ASSERT(Systems.ContainsKey(Type), Default, "System was not created");
		Systems.Remove(Type);
	}

	void Application::Run()
	{
		Initialize();
		Execute();
		Shutdown();
	}

	void Application::Initialize()
	{
		OnInitialize(Bootstrap);
		Bootstrap.RunBoot();
	}

	void Application::Shutdown()
	{
		OnShutdown(Bootstrap);
		Bootstrap.RunUnboot();
	}

	void Application::Execute()
	{
		OnExecute(Ticks);
		Ticks.Run();

		NxFr::Stopwatch Stopwatch;
		float DeltaTime = 0.0;

		while (IsRunning())
		{
			Stopwatch.Start();

			Ticks.Tick(DeltaTime);

			DeltaTime = (float)Stopwatch.Stop(NxFr::Time::SecondToMilli);

			DebugManager::GetInstance()->Flush();
		}
	}
}
