#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	void NexusAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::OnInitialize(Bootstrap);
	}

	void NexusAppApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap)
	{
		NexusEngineApplication::OnShutdown(Unbootstrap);
	}
}
