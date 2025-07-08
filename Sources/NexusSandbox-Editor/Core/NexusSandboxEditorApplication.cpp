#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	NexusSandboxEditorApplication::NexusSandboxEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEditorApplication(ProjectInfo)
	{
	}

	void NexusSandboxEditorApplication::OnInitialize()
	{
		NexusEditorApplication::OnInitialize();
	}

	void NexusSandboxEditorApplication::OnShutdown()
	{
		NexusEditorApplication::OnShutdown();
	}

	void NexusSandboxEditorApplication::OnExecute()
	{
		NexusEditorApplication::OnExecute();
	}
}
