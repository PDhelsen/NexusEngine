#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(System)

	void System::OnInitialize()
	{
		SetTickable(true);
	}

	void System::OnShutdown()
	{
	}

	void System::OnTick(float TimeStep)
	{
	}
}
