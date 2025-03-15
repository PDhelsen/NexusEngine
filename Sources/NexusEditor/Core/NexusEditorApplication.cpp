#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	void NexusEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrapper, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnInitialize(Bootstrapper, Systems);
	}

	void NexusEditorApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrapper, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnShutdown(Unbootstrapper, Systems);
	}

	void NexusEditorApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		NexusEngineApplication::OnExecute(Ticks, Systems);
	}
}
