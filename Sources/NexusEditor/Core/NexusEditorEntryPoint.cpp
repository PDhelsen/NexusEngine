#include "NexusEditor/Core/NexusEditorEntryPoint.h"
#include "NexusFramework/Core/NexusFramework.h"

namespace NxEd
{
	void Main()
	{
		NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-Editor", "HelloWorldEditor").Invoke();
		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
