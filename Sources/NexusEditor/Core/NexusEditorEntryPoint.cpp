#include "NexusEditor/Core/NexusEditorEntryPoint.h"

#include "NexusFramework/Core/NexusFramework.h"

namespace NxEd
{
	namespace EntryPoint
	{
		void Run()
		{
			NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-Editor", "Run").Invoke();
			NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
		}
	}
}
