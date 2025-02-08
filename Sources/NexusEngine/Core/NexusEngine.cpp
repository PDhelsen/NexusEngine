#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngine.h"
#include "NexusFramework/Memory/MemoryOperator.h"

namespace NxEn
{
	void HelloWorld()
	{
#if NEXUS_EDITOR
		NEXUS_LOG(Info, Default, "Engine - Hello World (Editor)");
#else
		NEXUS_LOG(Info, Default, "Engine - Hello World (App)");
#endif
	}
}
