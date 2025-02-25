#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	void NexusSandboxAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::OnInitialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[Sandbox-App] Hello World"); });
	}

	void NexusSandboxAppApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::OnShutdown(Bootstrap);
	}
}
