#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Debug/DebugSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(DebugSystem)

	void NxEn::DebugSystem::OnTick()
	{
		TickDebugGlobals();
	}

	void DebugSystem::TickDebugGlobals() const
	{
		NxFr::Stats::GetInstance()->Flush();
		NxFr::Logger::GetInstance()->Flush();
	}
}
