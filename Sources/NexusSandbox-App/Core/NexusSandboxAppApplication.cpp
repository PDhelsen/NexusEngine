#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	void NexusSandboxAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		NexusAppApplication::OnInitialize(Bootstrap, Systems);
	}

	void NexusSandboxAppApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		NexusAppApplication::OnShutdown(Unbootstrap, Systems);
	}

	void NexusSandboxAppApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusAppApplication::OnExecute(Ticks, Systems);
	}
}
