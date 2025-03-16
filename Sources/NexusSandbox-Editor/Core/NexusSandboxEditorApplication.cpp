#include "NexusSandbox-Editor/Core/NexusSandboxEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSE::NexusSandboxEditorApplication)

	void NexusSandboxEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnInitialize(Bootstrap, Systems);
	}

	void NexusSandboxEditorApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnShutdown(Unbootstrap, Systems);
	}

	void NexusSandboxEditorApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEditorApplication::OnExecute(Ticks, Systems);
	}
}
