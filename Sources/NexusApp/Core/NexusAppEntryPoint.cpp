#include "NexusApp/Core/NexusAppEntryPoint.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

#include "NexusEngine/Core/NexusEngine.h"

#include "NexusApp/Core/NexusApp.h"

namespace NxAp
{
	void Main()
	{
		NxFr::Initialize();

		NxEn::HelloWorld();
		HelloWorld();

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

		NxFr::Shutdown();
	}
}
