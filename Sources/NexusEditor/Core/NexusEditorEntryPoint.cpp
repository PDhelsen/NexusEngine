#include "NexusEditor/Core/NexusEditorEntryPoint.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxEd
{
	int Main(int argc, char* argv[])
	{
		NxFr::Initialize();

		NEXUS_LOG(Info, Default, "Args count: %d", argc);
		for (uint64 Index = 0; Index < argc; Index++)
			NEXUS_LOG(Info, Default, "%d: %s", Index, argv[Index]);

		NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-Editor", "HelloWorldEditor").Invoke();
		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

		NxFr::Shutdown();

		return 0;
	}
}
