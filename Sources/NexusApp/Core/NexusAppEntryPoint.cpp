#include "NexusApp/Core/NexusAppEntryPoint.h"

#include "NexusFramework/Core/NexusFramework.h"

namespace NxAp
{
	namespace EntryPoint
	{
		void Run()
		{
			NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-App", "Run").Invoke();
			NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
		}
	}
}
