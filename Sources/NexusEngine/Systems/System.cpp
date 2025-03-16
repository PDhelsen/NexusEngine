#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(System)

	uint64 FrameCount = 0;

	void System::OnInitialize()
	{
		SetTickable(true);
	}

	void System::OnShutdown()
	{
	}

	void System::OnTick(float TimeStep)
	{
		NEXUS_LOG(Info, Default, "Tick %.2f - %s - %i", TimeStep, GetObjectType().C(), TimeManager::GetInstance()->GetFrameIndex());

		if (FrameCount == 5)
		{
			Application::GetInstance()->GetTicker().AppendTickOnceCallback([]() { TimeManager::GetInstance()->SetMultiplier(0.1f); });
		}

		if (FrameCount++ > 10)
		{
			Application::GetInstance()->Quit();
		}
	}
}
