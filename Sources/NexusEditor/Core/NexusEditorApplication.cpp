#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication()
	{
	}

	NexusEditorApplication::~NexusEditorApplication()
	{
	}

	void NexusEditorApplication::Run()
	{
		NEXUS_LOG(Info, Default, "[Editor] Hello World");
		NexusEngineApplication::Run();
	}
}
