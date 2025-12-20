#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(WorldSystem)

	WorldSystem::WorldSystem()
		: OnWorldEvent(), OnGameObjectEvent(), Worlds()
	{
	}

	WorldSystem::~WorldSystem()
	{
	}

	World* WorldSystem::CreateWorld(NxFr::GUID WorldId, NxFr::StringView Name)
	{
		if (Worlds.ContainsKey(WorldId))
		{
			NEXUS_LOG(Warning, System, "World %d already exist", WorldId);
			return GetWorld(WorldId);
		}

		World* Instance = new World(WorldId);
		Instance->Initialize();
		Instance->SetName(Name);
		Worlds.Append(WorldId, Instance);

		GetOnWorldEvent().Invoke(AppendedId, WorldId);

		return Instance;
	}

	World* WorldSystem::GetWorld(NxFr::GUID WorldId)
	{
		World** Instance = Worlds.TryGet(WorldId);
		if (!Instance)
		{
			NEXUS_LOG(Error, System, "World %d doesn't exist", WorldId);
			return nullptr;
		}

		return *Instance;
	}

	NxFr::Array<NxFr::GUID> WorldSystem::GetWorlds()
	{
		NxFr::Array<NxFr::GUID> Result(Worlds.GetCount());
		uint64 Index = 0;

		for (auto& [Id, Instance] : Worlds)
		{
			Result[Index++] = Id;
		}

		return Result;
	}

	void WorldSystem::DestroyWorld(NxFr::GUID WorldId)
	{
		if (Worlds.ContainsKey(WorldId))
		{
			NEXUS_LOG(Warning, System, "World %d doesn't exist", WorldId);
			return;
		}

		GetOnWorldEvent().Invoke(RemovedId, WorldId);

		World* Instance = Worlds[WorldId];
		Worlds.Remove(WorldId);
		Instance->Shutdown();
		delete Instance;
	}

	void WorldSystem::OnInitialize()
	{
		System::OnInitialize();

		CreateWorld(WorldId, WorldId);
	}

	void WorldSystem::OnShutdown()
	{
		DestroyWorld(WorldId);

		System::OnShutdown();
	}

	void WorldSystem::OnTick(float TimeStep)
	{
		for (auto& [Id, Instance] : Worlds)
		{
			Instance->Tick(TimeStep);
		}
	}
}
