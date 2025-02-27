#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		Bootstrap.AddStep([]() { NxFr::Initialize(); });
		Bootstrap.AddStep([]() {
#if NEXUS_EDITOR
			NEXUS_LOG(Info, Default, "[Engine] Hello World (Editor)");
#else
			NEXUS_LOG(Info, Default, "[Engine] Hello World (App)");
#endif
		});

		Bootstrap.AddSystem<System>();
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		Bootstrap.AddStep([]() { NxFr::Shutdown(); });
	}
}
