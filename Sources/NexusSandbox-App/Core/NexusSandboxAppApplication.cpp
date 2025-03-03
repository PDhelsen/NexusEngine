#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	void NexusSandboxAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::OnInitialize(Bootstrap);
	}

	void NexusSandboxAppApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::OnShutdown(Bootstrap);
	}
}
