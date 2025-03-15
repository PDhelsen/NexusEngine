#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	void NexusSandboxAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrapper, NxEn::SystemManager& Systems)
	{
		NexusAppApplication::OnInitialize(Bootstrapper, Systems);
	}

	void NexusSandboxAppApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrapper, NxEn::SystemManager& Systems)
	{
		NexusAppApplication::OnShutdown(Unbootstrapper, Systems);
	}

	void NexusSandboxAppApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusAppApplication::OnExecute(Ticks, Systems);
	}
}
