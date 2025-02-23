#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	void NexusAppApplication::Initialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::Initialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[App] Hello World"); });
	}

	void NexusAppApplication::Shutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::Shutdown(Bootstrap);
	}
}
