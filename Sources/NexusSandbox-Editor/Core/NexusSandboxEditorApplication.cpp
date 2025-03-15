#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	void NexusSandboxEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrapper, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnInitialize(Bootstrapper, Systems);
	}

	void NexusSandboxEditorApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrapper, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnShutdown(Unbootstrapper, Systems);
	}

	void NexusSandboxEditorApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnExecute(Ticks, Systems);
	}
}
