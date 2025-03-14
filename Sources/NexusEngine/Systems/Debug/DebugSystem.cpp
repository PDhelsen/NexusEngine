#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/DebugSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(DebugSystem)

	void DebugSystem::OnTick(float TimeStep)
	{
		NEXUS_LOG(Info, Default, "Tick (%.5f)", TimeStep);
		FrameCount++;

		if (FrameCount > 10)
		{
			Application::GetInstance()->Quit();
		}

		TickDebugGlobals();
	}

	void DebugSystem::TickDebugGlobals() const
	{
		NxFr::Stats::GetInstance()->Flush();
		NxFr::Logger::GetInstance()->Flush();
	}
}
