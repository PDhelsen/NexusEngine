#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	static Application* Instance = nullptr;

	Application* Application::GetInstance()
	{
		return Instance;
	}

	Application::Application()
		: Bootstrap(), Systems(), WantsToQuit(false)
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

	bool Application::IsRunning()
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
		for (auto It = Systems.Begin(); It != Systems.End() && IsRunning(); ++It)
		{
			It->Value->Tick();
		}

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
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
}
