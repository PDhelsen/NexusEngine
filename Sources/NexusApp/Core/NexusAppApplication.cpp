#include "NexusApp/Core/NexusAppApplication.h"

namespace NxAp
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxAp::NexusAppApplication)

	void NexusAppApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
	}

	void NexusAppApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
	}

	void NexusAppApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
	}
}
