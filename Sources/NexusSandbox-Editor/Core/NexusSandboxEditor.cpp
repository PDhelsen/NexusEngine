#include "NexusSandbox-Editor/Core/NexusSandboxEditor.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusEngine/Core/NexusEngine.h"
#include "NexusEditor/Core/NexusEditor.h"


namespace NxSE
{
	void HelloWorld()
	{
		NxEn::HelloWorld();
		NxEd::HelloWorld();

		NEXUS_LOG(Info, Default, "Sandbox - Hello World (Editor)");
	}
}
