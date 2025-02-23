#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	void NexusSandboxEditorApplication::Initialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEditorApplication::Initialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[Sandbox-Editor] Hello World"); });
	}

	void NexusSandboxEditorApplication::Shutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEditorApplication::Shutdown(Bootstrap);
	}
}
