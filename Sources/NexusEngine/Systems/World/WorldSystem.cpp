#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"
#include "NexusEngine/Systems/World/WorldManager.h"

namespace NxEn
{
	WorldSystem::WorldSystem()
		: Manager(nullptr)
	{
	}

	WorldSystem::~WorldSystem()
	{
	}

	void WorldSystem::OnInitialize()
	{
		System::OnInitialize();

		Manager = new WorldManager();
	}

	void WorldSystem::OnShutdown()
	{
		System::OnShutdown();

		delete Manager;
	}

	void WorldSystem::OnTick(float TimeStep)
	{
		Manager->Tick(TimeStep);
	}
}
