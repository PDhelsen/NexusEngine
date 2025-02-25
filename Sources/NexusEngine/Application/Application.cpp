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
		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
