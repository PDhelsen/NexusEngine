#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Application.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxEn
{
	Application::Application()
	{
		NxFr::Initialize();
	}

	Application::~Application()
	{
		NxFr::Shutdown();
	}

	void Application::Run()
	{
	}
}
