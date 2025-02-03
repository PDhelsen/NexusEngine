#include "NexusApp/Core/NexusApp.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

#include "NexusEngine/Core/NexusEngine.h"

namespace NxAp
{
	void HelloWorld()
	{
		NEXUS_LOG(Info, Default, "App - Hello World 5");
	}
}

int main()
{
	NxFr::Initialize();

	NxAp::HelloWorld();
	NxEn::HelloWorld();

	NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

	NxFr::Shutdown();
}
