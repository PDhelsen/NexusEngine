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
		Bootstrap.Boot();
	}

	void Application::Shutdown()
	{
		OnShutdown(Bootstrap);
		Bootstrap.Unboot();
	}

	void Application::Run()
	{
		for (auto& It : Systems)
		{
			It.Value->Tick();
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
