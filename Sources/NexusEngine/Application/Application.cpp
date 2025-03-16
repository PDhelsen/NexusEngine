#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

#include "NexusEngine/Misc/DebugManager.h"
#include "NexusEngine/Misc/TimeManager.h"

namespace NxEn
{
	static Application* Instance = nullptr;

	Application* Application::GetInstance()
	{
		return Instance;
	}

	Application::Application()
		: WantsToQuit(false)
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

		TimeManager::GetInstance()->Run();

		while (IsRunning())
		{
			Ticks.Tick();

			DebugManager::GetInstance()->Flush();
			TimeManager::GetInstance()->Tick();

			// TEMP: Avoid looping too fast for now since the app is empty
			NxFr::Platform::GetInstance()->Sleep(1);
		}
	}
}
