#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	NexusSandboxAppApplication::NexusSandboxAppApplication(const NxEn::Project& ProjectInfo)
		: NexusAppApplication(ProjectInfo)
	{
	}

	NexusSandboxAppApplication::~NexusSandboxAppApplication()
	{
	}

	void NexusSandboxAppApplication::OnInitialize()
	{
		NexusAppApplication::OnInitialize();
	}

	void NexusSandboxAppApplication::OnShutdown()
	{
		NexusAppApplication::OnShutdown();
	}

	void NexusSandboxAppApplication::OnExecute()
	{
		NexusAppApplication::OnExecute();
	}
}
