#include "NexusSandbox/Core/NexusSandbox.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngine.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxSb
{
	void HelloWorld()
	{
		NEXUS_LOG(Info, Default, "Sandbox - Hello World");
	}
}

int main()
{
	NxFr::Initialize();

	NxEn::HelloWorld();
	NxSb::HelloWorld();
	NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

	NxFr::Shutdown();
}
