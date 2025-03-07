#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/DebugSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(DebugSystem)

	void DebugSystem::OnTick()
	{
		TickDebugGlobals();
	}

	void DebugSystem::TickDebugGlobals() const
	{
		NxFr::Stats::GetInstance()->Flush();
		NxFr::Logger::GetInstance()->Flush();
	}
}
