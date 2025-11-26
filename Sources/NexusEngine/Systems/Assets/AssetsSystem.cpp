#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsSystem)

	void AssetsSystem::OnInitialize()
	{
		System::OnInitialize();
	}

	void AssetsSystem::OnShutdown()
	{
		System::OnShutdown();
	}

	void AssetsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
	}
}
