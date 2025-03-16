#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	void NexusEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnInitialize(Bootstrap, Systems);
	}

	void NexusEditorApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnShutdown(Unbootstrap, Systems);
	}

	void NexusEditorApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnExecute(Ticks, Systems);
	}
}
