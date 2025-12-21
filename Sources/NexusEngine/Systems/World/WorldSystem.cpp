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
		if (!Worlds.ContainsKey(WorldId))
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


	Prefab* WorldSystem::CreatePrefab(NxFr::Handle<GameObject> Target, NxFr::StringView Path)
	{
		NxEn::Prefab* Instance = Application::GetSystem<AssetsSystem>()->Create<NxEn::Prefab>(Path, "prefab");
		Instance->SetRoot(Target);
		return Instance;
	}

	void WorldSystem::SavePrefab(NxFr::Handle<GameObject> Target)
	{
		NxEn::Prefab* Instance = Application::GetSystem<AssetsSystem>()->Load<NxEn::Prefab>(Target->GetReferenceId());
		Instance->SetRoot(Target);
	}

	void WorldSystem::OnInitialize()
	{
		System::OnInitialize();

#if NEXUS_EDITOR
		CreateWorld(DummyId, DummyId);
#endif
		CreateWorld(WorldId, WorldId);
	}

	void WorldSystem::OnShutdown()
	{
#if NEXUS_EDITOR
		DestroyWorld(DummyId);
#endif
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
