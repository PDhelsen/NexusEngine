#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	NexusEngineApplication::NexusEngineApplication()
	{
		NxFr::Initialize();
	}

	NexusEngineApplication::~NexusEngineApplication()
	{
		NxFr::Shutdown();
	}

	void NexusEngineApplication::Run()
	{
#if NEXUS_EDITOR
		NEXUS_LOG(Info, Default, "[Engine] Hello World (Editor)");
#else
		NEXUS_LOG(Info, Default, "[Engine] Hello World (App)");
#endif

		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
