#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"
#include "NexusEngine/Systems/World/WorldManager.h"

namespace NxEn
{
	static Command* CmdWorldWorldCreate = Command::Create("World.World.Create"_Sid, "Create World", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Name)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		World->CreateWorld(Name);
	}));
	static Command* CmdWorldWorldDestroy = Command::Create("World.World.Destroy"_Sid, "Destroy World", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		World->DestroyWorld(NxFr::StringUtility::FromString<NxFr::GUID>(Id));
	}));
	static Command* CmdWorldGameObjectCreate = Command::Create("World.GameObject.Create"_Sid, "Create GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Name, NxFr::StringView WorldId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		World->CreateGameObject(Name, NxFr::Handle<GameObject>(), NxFr::StringId(WorldId));
	}));
	static Command* CmdWorldGameObjectDuplicate = Command::Create("World.GameObject.Duplicate"_Sid, "Duplicate GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(Id));
		World->DuplicateGameObject(Instance);
	}));
	static Command* CmdWorldGameObjectDestroy = Command::Create("World.GameObject.Destroy"_Sid, "Destroy GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(Id));
		World->DestroyGameObject(Instance);
	}));
	static Command* CmdWorldGameObjectAttach = Command::Create("World.GameObject.Attach"_Sid, "Attach GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView InstanceId, NxFr::StringView ParentId, NxFr::StringView Index)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(InstanceId));
		NxFr::Handle<GameObject> Parent = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ParentId));
		World->AttachGameObject(Instance, Parent, NxFr::StringUtility::FromString<uint64>(Index));
	}));
	static Command* CmdWorldBehaviourCreate = Command::Create("World.Behaviour.Create"_Sid, "Create Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->CreateBehaviour(Type, Target);
	}));
	static Command* CmdWorldBehaviourDestroy = Command::Create("World.Behaviour.Destroy"_Sid, "Destroy Behaviour", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView BehaviourId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Behaviour> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(BehaviourId));
		World->DestroyBehaviour(Instance);
	}));
	static Command* CmdWorldComponentCreate = Command::Create("World.Component.Create"_Sid, "Create Component", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->CreateComponent(Type, Target);
	}));
	static Command* CmdWorldComponentDestroy = Command::Create("World.Component.Destroy"_Sid, "Destroy Component", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView ComponentId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Component> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ComponentId));
		World->DestroyComponent(Instance);
	}));

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

		OnWorldChange.Invoke(EventCreatedId, Name);

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

		OnWorldChange.Invoke(EventDestroyedId, WorldId);

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

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetWorldId(), Instance->GetId());

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

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance)
		{
			return;
		}

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetWorldId(), Instance->GetId());

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

		OnWorldObjectChange.Invoke(EventMovedId, Instance->GetWorldId(), Instance->GetId());
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

		OnWorldObjectChange.Invoke(EventMovedId, Instance->GetWorldId(), Instance->GetId());
	}

	NxFr::Handle<Behaviour> WorldSystem::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");

		WorldManager* Manager = GetManager(Target->GetWorldId());
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", Target->GetWorldId());
			return NxFr::Handle<Behaviour>();
		}

		NxFr::Handle<Behaviour> Instance = Manager->CreateBehaviour(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		if (!Instance)
		{
			return;
		}

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		WorldManager* Manager = GetManager(Instance->GetGameObject()->GetWorldId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyBehaviour(Instance);
	}

	NxFr::Handle<Component> WorldSystem::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");

		WorldManager* Manager = GetManager(Target->GetWorldId());
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", Target->GetWorldId());
			return NxFr::Handle<Component>();
		}

		NxFr::Handle<Component> Instance = Manager->CreateComponent(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		if (!Instance)
		{
			return;
		}

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		WorldManager* Manager = GetManager(Instance->GetGameObject()->GetWorldId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyComponent(Instance);
	}

	bool WorldSystem::Belong(NxFr::Handle<Object> Instance, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return false;
		}

		return Manager->Belong(Instance);
	}

	NxFr::Array<NxFr::Handle<Object>> WorldSystem::Find(NxFr::StringView Query, WorldObjectType Type, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Array<NxFr::Handle<Object>>();
		}

		return Manager->Find(Query, Type);
	}

	NxFr::Array<NxFr::Handle<Object>> WorldSystem::GetObjects(WorldObjectType Type, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Array<NxFr::Handle<Object>>();
		}

		return Manager->GetObjects(Type);
	}

	NxFr::Handle<Object> WorldSystem::GetObject(NxFr::GUID ObjectId, NxFr::GUID WorldId)
	{
		if (WorldId != 0)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return NxFr::Handle<Object>();
			}

			return Manager->GetObject(ObjectId);
		}
		else
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
	}

	Iterator::WorldObject WorldSystem::Begin(NxFr::StringId Type, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return Iterator::WorldObject(nullptr, 0);
		}

		return Manager->Begin(Type);
	}

	Iterator::WorldObject WorldSystem::End(NxFr::StringId Type, NxFr::GUID WorldId)
	{
		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return Iterator::WorldObject(nullptr, 0);
		}

		return Manager->End(Type);
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
}
