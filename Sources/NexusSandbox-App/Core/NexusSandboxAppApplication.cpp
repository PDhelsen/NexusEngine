#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	NexusSandboxAppApplication::NexusSandboxAppApplication()
	{
	}

	NexusSandboxAppApplication::~NexusSandboxAppApplication()
	{
	}

	void NexusSandboxAppApplication::Run()
	{
		NEXUS_LOG(Info, Default, "[Sandbox-App] Hello World");
		NexusAppApplication::Run();
	}
}
