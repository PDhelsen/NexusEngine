#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	void NexusSandboxEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEditorApplication::OnInitialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[Sandbox-Editor] Hello World"); });
	}

	void NexusSandboxEditorApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEditorApplication::OnShutdown(Bootstrap);
	}
}
