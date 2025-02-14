#include "NexusSandbox-Editor/Core/NexusSandboxEditor.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusEngine/Core/NexusEngine.h"
#include "NexusApp/Core/NexusApp.h"
#include "NexusEditor/Core/NexusEditor.h"

#include "NexusSandbox-App/Core/NexusSandboxApp.h"

namespace NxSE
{
	void HelloWorld()
	{
		NxEd::HelloWorld();

		NEXUS_LOG(Info, Default, "Sandbox - Hello World (Editor)");
	}
}
