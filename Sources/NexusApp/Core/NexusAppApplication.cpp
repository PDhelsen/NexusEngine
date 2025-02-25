#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	void NexusAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::OnInitialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[App] Hello World"); });
	}

	void NexusAppApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::OnShutdown(Bootstrap);
	}
}
