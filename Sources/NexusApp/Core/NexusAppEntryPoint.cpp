#include "NexusApp/Core/NexusAppEntryPoint.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

#include "NexusEngine/Core/NexusEngine.h"

#include "NexusApp/Core/NexusApp.h"

namespace NxAp
{
	int Main(int argc, char* argv[])
	{
		NxFr::Initialize();

		NEXUS_LOG(Info, Default, "Args count: %d", argc);
		for (uint64 Index = 0; Index < argc; Index++)
			NEXUS_LOG(Info, Default, "%d: %s", Index, argv[Index]);

		NxEn::HelloWorld();
		HelloWorld();

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

		NxFr::Shutdown();

		return 0;
	}
}
