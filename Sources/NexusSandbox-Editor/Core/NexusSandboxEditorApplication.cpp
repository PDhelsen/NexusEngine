#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	NexusSandboxEditorApplication::NexusSandboxEditorApplication()
	{
	}

	NexusSandboxEditorApplication::~NexusSandboxEditorApplication()
	{
	}

	void NexusSandboxEditorApplication::Run()
	{
		NEXUS_LOG(Info, Default, "[Sandbox-Editor] Hello World");
		NexusEditorApplication::Run();
	}
}
