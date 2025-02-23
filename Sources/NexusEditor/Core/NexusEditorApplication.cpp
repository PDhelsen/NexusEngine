#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	void NexusEditorApplication::Initialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::Initialize(Bootstrap);

		Bootstrap.AddStep([]() { NEXUS_LOG(Info, Default, "[Editor] Hello World"); });
	}

	void NexusEditorApplication::Shutdown(NxEn::Bootstrapper& Bootstrap)
	{
		NexusEngineApplication::Shutdown(Bootstrap);
	}
}
