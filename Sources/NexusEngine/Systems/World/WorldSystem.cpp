#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(WorldSystem)

	World* WorldSystem::CreateWorld(NxFr::StringId Id)
	{
		if (Worlds.ContainsKey(Id))
		{
			NEXUS_LOG(Warning, System, "World %s already exist", Id.C());
			return GetWorld(Id);
		}

		World* Instance = Worlds.Append(Id, new World(Id));
		Instance->Initialize();

		GetOnWorldEvent().Invoke(AppendedId, Id);
		return Instance;
	}

	World* WorldSystem::GetWorld(NxFr::StringId Id)
	{
		World** Instance = Worlds.TryGet(Id);
		if (!Instance)
		{
			NEXUS_LOG(Error, System, "World %s doesn't exist", Id.C());
			return nullptr;
		}

		return *Instance;
	}

	NxFr::Array<NxFr::StringId> WorldSystem::GetWorlds()
	{
		NxFr::Array<NxFr::StringId> Result(Worlds.GetCount());
		uint64 Index = 0;

		for (auto& [Id, Instance] : Worlds)
		{
			Result[Index++] = Id;
		}

		return Result;
	}

	void WorldSystem::DestroyWorld(NxFr::StringId Id)
	{
		if (Worlds.ContainsKey(Id))
		{
			NEXUS_LOG(Warning, System, "World %s doesn't exist", Id.C());
			return;
		}

		GetOnWorldEvent().Invoke(RemovedId, Id);

		World* Instance = Worlds[Id];
		Instance->Shutdown();
		Worlds.Remove(Id);
	}

	void WorldSystem::OnInitialize()
	{
		System::OnInitialize();

		CreateWorld(MainId);
	}

	void WorldSystem::OnShutdown()
	{
		DestroyWorld(MainId);

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
