#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	void NexusEditorApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::OnInitialize(Bootstrap);
	}

	void NexusEditorApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::OnShutdown(Bootstrap);
	}
}
