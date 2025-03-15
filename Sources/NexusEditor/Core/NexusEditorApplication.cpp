#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	void NexusEditorApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
	}

	void NexusEditorApplication::OnShutdown()
	{
		NexusEngineApplication::OnShutdown();
	}

	void NexusEditorApplication::OnExecute()
	{
		NexusEngineApplication::OnExecute();
	}
}
