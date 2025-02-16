#include "ApplicationApp.h"

#include "NexusFramework/Core/NexusFramework.h"

namespace NxAp
{
	ApplicationApp::ApplicationApp()
	{
	}

	ApplicationApp::~ApplicationApp()
	{
	}

	void ApplicationApp::Run()
	{
		NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-App", "Run").Invoke();
		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
