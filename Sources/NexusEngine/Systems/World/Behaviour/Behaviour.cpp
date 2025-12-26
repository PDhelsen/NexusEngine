#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Behaviour/Behaviour.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Behaviour)

	Behaviour::Behaviour()
	{
		SetTickable(true);
	}

	Behaviour::~Behaviour()
	{
	}

	void Behaviour::Tick(float TimeStep)
	{
		if (!IsTickable() || !IsEnabled() || !Target->IsTickable() || !Target->IsEnabled())
		{
			return;
		}

		OnTick(TimeStep);
	}
}
