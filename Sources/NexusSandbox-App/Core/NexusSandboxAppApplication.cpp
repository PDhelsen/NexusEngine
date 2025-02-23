#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	void NexusSandboxAppApplication::Initialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::Initialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[Sandbox-App] Hello World"); });
	}

	void NexusSandboxAppApplication::Shutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::Shutdown(Bootstrap);
	}
}
