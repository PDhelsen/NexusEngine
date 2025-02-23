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

	void Application::Run()
	{
		Initialize(Bootstrap);
		Bootstrap.Boot();

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

		Shutdown(Bootstrap);
		Bootstrap.Unboot();
	}
}
