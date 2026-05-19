#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NX_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	NexusSandboxEditorApplication::NexusSandboxEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEditorApplication(ProjectInfo)
	{
	}

	NexusSandboxEditorApplication::~NexusSandboxEditorApplication()
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
