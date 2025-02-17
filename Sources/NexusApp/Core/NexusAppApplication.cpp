#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	NexusAppApplication::NexusAppApplication()
	{
	}

	NexusAppApplication::~NexusAppApplication()
	{
	}

	void NexusAppApplication::Run()
	{
		NEXUS_LOG(Info, Default, "[App] Hello World");
		NexusEngineApplication::Run();
	}
}
