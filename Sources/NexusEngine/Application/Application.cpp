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
		: Bootstrap(), Systems(1), WantsToQuit(false)
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

	void Application::Execute()
	{
		Initialize();
		Run();
		Shutdown();
	}

	void Application::Initialize()
	{
		OnInitialize(Bootstrap);
		Bootstrap.Boot();
	}

	void Application::Shutdown()
	{
		OnShutdown(Bootstrap);
		Bootstrap.Unboot();
	}

	void Application::Run()
	{
		for (auto It = Systems.Begin(); It != Systems.End() && IsRunning(); ++It)
		{
			It->Value->Tick();
		}

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}

	void Application::RegisterSystem(NxFr::StringView Name, System* System)
	{
		NEXUS_ASSERT(!Systems.ContainsKey(Name), Default, "System is already created");
		Systems.Append(Name, System);
	}

	void Application::UnregisterSystem(NxFr::StringView Name)
	{
		NEXUS_ASSERT(Systems.ContainsKey(Name), Default, "System was not created");
		Systems.Remove(Name);
	}
}
