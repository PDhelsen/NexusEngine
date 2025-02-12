#include "NexusApp/Core/NexusAppEntryPoint.h"
#include "NexusFramework/Core/NexusFramework.h"

namespace NxAp
{
	void Main()
	{
		NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-App", "HelloWorldApp").Invoke();
		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
