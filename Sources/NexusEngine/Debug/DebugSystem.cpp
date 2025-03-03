#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Debug/DebugSystem.h"

namespace NxEn
{
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
