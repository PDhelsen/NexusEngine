#include "NexusSandbox-App/Core/NexusSandboxApp.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusEngine/Core/NexusEngine.h"
#include "NexusApp/Core/NexusApp.h"

namespace NxSb
{
	void HelloWorld()
	{
		NxEn::HelloWorld();
		NxAp::HelloWorld();

		NEXUS_LOG(Info, Default, "Sandbox - Hello World");
	}
}
