#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"

namespace NxSA
{
	NX_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

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

	void NexusSandboxAppApplication::OnRun()
	{
		NexusAppApplication::OnRun();
	}
}
