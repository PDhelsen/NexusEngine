#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"
#include "NexusEngine/Systems/World/WorldManager.h"

namespace NxEn
{
	WorldSystem::WorldSystem()
		: Managers()
	{
	}

	WorldSystem::~WorldSystem()
	{
	}

	World* WorldSystem::CreateWorld(NxFr::StringId Name)
	{
		if (GetManager(Name))
		{
			NX_LOG(Warning, System, "World %llu already exist", Name.GetId());
			return GetWorld(Name);
		}

		WorldManager* Manager = new WorldManager();
		Managers.Append(Name.GetId(), Manager);

		World* World = Manager->CreateWorld(Name);
		World->Initialize();
		World->SetEnabled(true);

		NxFr::Handle<GameObject> Root = CreateGameObject(Name, NxFr::Handle<GameObject>(), Name);
		Manager->SetWorldRoot(Root);

		return World;
	}

	void WorldSystem::DestroyWorld(NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return;
		}

		NxFr::Handle<GameObject> Root = Manager->GetWorld()->GetRoot();
		Manager->SetWorldRoot(NxFr::Handle<GameObject>());
		DestroyGameObject(Root);

		World* World = Manager->GetWorld();
		World->SetEnabled(false);
		World->Shutdown();

		Managers.Remove(WorldId);
		delete Manager;
	}

	World* WorldSystem::GetWorld(NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Error, System, "World %llu doesn't exist", WorldId);
			return nullptr;
		}

		return Manager->GetWorld();
	}

	NxFr::Array<NxFr::GUID> WorldSystem::GetWorlds()
	{
		NxFr::Array<NxFr::GUID> Result(Managers.GetCount());
		uint64 Index = 0;

		for (auto& [Id, Manager] : Managers)
		{
			Result[Index++] = Id;
		}

		return Result;
	}

	NxFr::Handle<GameObject> WorldSystem::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		NX_ASSERT(!Parent || Parent->GetWorldId() == WorldId, Default, "Parent should belong to the same world");

		if (!Parent)
		{
			Parent = Manager->GetWorld()->GetRoot();
		}

		NxFr::Handle<GameObject> Instance = Manager->CreateGameObject(Name, Parent);
		Instance->Initialize();
		Instance->SetEnabled(true);

		return Instance;
	}

	NxFr::Handle<GameObject> WorldSystem::DuplicateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Original != Manager->GetWorld()->GetRoot(), Default, "Can't duplicate root object");
		NX_ASSERT(!Parent || Parent->GetWorldId() == WorldId, Default, "Parent should belong to the same world");

		if (!Parent)
		{
			Parent = Original->GetParent();
		}
		if (!Parent)
		{
			Parent = Manager->GetWorld()->GetRoot();
		}

		NxFr::Handle<GameObject> Instance = Manager->DuplicateGameObject(Original, Parent);
		Instance->Initialize();
		Instance->UpdateHierarchy();

		return Instance;
	}

	void WorldSystem::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance)
		{
			return;
		}

		WorldManager* Manager = GetManager(Instance->GetWorldId());

		NX_ASSERT(Instance != Manager->GetWorld()->GetRoot(), Default, "Can't destroy root object");

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyGameObject(Instance);
	}

	void WorldSystem::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index)
	{
		if (!Parent)
		{
			DetachGameObject(Instance);
			return;
		}

		WorldManager* Manager = GetManager(Instance->GetWorldId());

		NX_ASSERT(Instance, Default, "Instance should be valid");
		NX_ASSERT(Instance != Manager->GetWorld()->GetRoot(), Default, "Can't attach root object");
		NX_ASSERT(!Parent || Parent->GetWorldId() == Instance->GetWorldId(), Default, "Parent should belong to the same world");

		NxFr::Handle<GameObject> Root = Manager->GetWorld()->GetRoot();
		if (!Parent)
		{
			Parent = Root;
		}
		if (Index < 0)
		{
			Index = Parent->GetChildCount();
		}

		Manager->AttachGameObject(Instance, Parent, Index);
		Instance->UpdateHierarchy();
	}

	void WorldSystem::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance)
		{
			return;
		}

		WorldManager* Manager = GetManager(Instance->GetWorldId());

		NX_ASSERT(Instance != Manager->GetWorld()->GetRoot(), Default, "Can't detach root object");

		Manager->DetachGameObject(Instance);
		Instance->UpdateHierarchy();
	}

	NxFr::Handle<Behaviour> WorldSystem::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<Behaviour>();
		}

		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Target->GetWorldId() == WorldId, Default, "Target should belong to the same world");

		NxFr::Handle<Behaviour> Instance = Manager->CreateBehaviour(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		return Instance;
	}

	void WorldSystem::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		if (!Instance)
		{
			return;
		}

		WorldManager* Manager = GetManager(Instance->GetGameObject()->GetWorldId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyGameObject(Instance);
	}

	NxFr::Handle<Component> WorldSystem::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<Component>();
		}

		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Target->GetWorldId() == WorldId, Default, "Target should belong to the same world");

		NxFr::Handle<Component> Instance = Manager->CreateComponent(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		return Instance;
	}

	void WorldSystem::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		if (!Instance)
		{
			return;
		}

		WorldManager* Manager = GetManager(Instance->GetGameObject()->GetWorldId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyGameObject(Instance);
	}

	bool WorldSystem::Belong(NxFr::Handle<Object> Instance, NxFr::GUID WorldId) const
	{
		const WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return false;
		}

		return Manager->Belong(Instance);
	}

	NxFr::Array<NxFr::Handle<Object>> WorldSystem::Find(NxFr::StringView Query, WorldObjectType Type, NxFr::GUID WorldId) const
	{
		const WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Array<NxFr::Handle<Object>>();
		}

		return Manager->Find(Query, Type);
	}

	NxFr::Array<NxFr::Handle<Object>> WorldSystem::GetObjects(WorldObjectType Type, NxFr::GUID WorldId) const
	{
		const WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Array<NxFr::Handle<Object>>();
		}

		return Manager->GetObjects(Type);
	}

	NxFr::Handle<Object> WorldSystem::GetObject(NxFr::GUID ObjectId) const
	{
		NxFr::Handle<Object> Result;

		for (auto [Id, Manager] : Managers)
		{
			Result = Manager->GetObject(ObjectId);
			if (Result)
			{
				break;
			}
		}

		return Result;
	}

	void WorldSystem::OnInitialize()
	{
		System::OnInitialize();

		CreateWorld(MainWorldId);
	}

	void WorldSystem::OnShutdown()
	{
		System::OnShutdown();

		DestroyWorld(MainWorldId);
	}

	void WorldSystem::OnTick(float TimeStep)
	{
		for (auto [Id, Manager] : Managers)
		{
			Manager->Tick(TimeStep);
		}
	}

	WorldManager* WorldSystem::GetManager(NxFr::GUID WorldId)
	{
		WorldManager** Manager = Managers.TryGet(WorldId);
		return Manager ? *Manager : nullptr;
	}

	const WorldManager* WorldSystem::GetManager(NxFr::GUID WorldId) const
	{
		WorldManager* const * Manager = Managers.TryGet(WorldId);
		return Manager ? *Manager : nullptr;
	}
}
