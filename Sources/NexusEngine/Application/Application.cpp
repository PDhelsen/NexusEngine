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
		: Bootstrap(), Systems(1)
	{
		NEXUS_ASSERT(Instance == nullptr, Default, "Application was already created");
		Instance = this;
	}

	Application::~Application()
	{
		Instance = nullptr;
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
		Bootstrap.Boot(&Systems);
	}

	void Application::Shutdown()
	{
		OnShutdown(Bootstrap);
		Bootstrap.Unboot(&Systems);
	}

	void Application::Run()
	{
		for (auto System : Systems)
		{
			System->Tick();
		}

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
