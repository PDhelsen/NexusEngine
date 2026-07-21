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
	static Command* CmdWorldWorldDestroy = Command::Create("World.World.Destroy"_Sid, "Destroy World", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView WorldId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		World->DestroyWorld(NxFr::StringUtility::FromString<NxFr::GUID>(WorldId));
	}));
	static Command* CmdWorldPrefabPack = Command::Create("World.Prefab.Pack"_Sid, "Create Prefab", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView GameObjectId, NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* World = Application::GetSystem<WorldSystem>();

		NxFr::GUID Id = Assets->PathToId(Path);
		Prefab* PrefabInstance = Id == Object::NullId ? Assets->Create<Prefab>(Path, Prefab::Extension) : Assets->Load<Prefab>(Id);
		NxFr::Handle<GameObject> GameObjectInstance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		PrefabInstance->SetRoot(GameObjectInstance);
	}));
	static Command* CmdWorldPrefabUnpack = Command::Create("World.Prefab.Unpack"_Sid, "Create Prefab", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView GameObjectId, NxFr::StringView Path)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> GameObjectInstance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		World->UnpackPrefab(GameObjectInstance);
	}));
	static Command* CmdWorldGameObjectCreate = Command::Create("World.GameObject.Create"_Sid, "Create GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Name, NxFr::StringView WorldId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		World->CreateGameObject(Name, NxFr::Handle<GameObject>(), NxFr::StringId(WorldId));
	}));
	static Command* CmdWorldGameObjectInstantiate = Command::Create("World.GameObject.Instantiate"_Sid, "Instantiate GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView GameObjectId)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* World = Application::GetSystem<WorldSystem>();

		NxFr::GUID Id = NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId);
		if (Assets->IsTracked(Id))
		{
			Prefab* PrefabInstance = Assets->Load<Prefab>(Id);
			World->InstantiateGameObject(PrefabInstance->GetRoot());
		}
		else
		{
			NxFr::Handle<GameObject> GameObjectInstance = World->GetObject(Id);
			World->InstantiateGameObject(GameObjectInstance);
		}
	}));
	static Command* CmdWorldGameObjectDuplicate = Command::Create("World.GameObject.Duplicate"_Sid, "Duplicate GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView GameObjectId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		World->DuplicateGameObject(Instance);
	}));
	static Command* CmdWorldGameObjectDestroy = Command::Create("World.GameObject.Destroy"_Sid, "Destroy GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView GameObjectId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		World->DestroyGameObject(Instance);
	}));
	static Command* CmdWorldGameObjectAttach = Command::Create("World.GameObject.Attach"_Sid, "Attach GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView GameObjectId, NxFr::StringView ParentId, NxFr::StringView Index)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		NxFr::Handle<GameObject> Parent = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ParentId));
		World->AttachGameObject(Instance, Parent, NxFr::StringUtility::FromString<uint64>(Index));
	}));
	static Command* CmdWorldBehaviourCreate = Command::Create("World.Behaviour.Create"_Sid, "Create Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->CreateBehaviour(Type, Target);
	}));
	static Command* CmdWorldBehaviourInstantiate = Command::Create("World.Behaviour.Instantiate"_Sid, "Instantiate Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView BehaviourId, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Behaviour> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(BehaviourId));
		NxFr::Handle<Behaviour> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->InstantiateBehaviour(Instance, Target);
	}));
	static Command* CmdWorldBehaviourDuplicate = Command::Create("World.Behaviour.Duplicate"_Sid, "Duplicate Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView BehaviourId, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Behaviour> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(BehaviourId));
		NxFr::Handle<Behaviour> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->DuplicateBehaviour(Instance, Target);
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
	static Command* CmdWorldComponentInstantiate = Command::Create("World.Component.Instantiate"_Sid, "Instantiate Component", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView ComponentId, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Component> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ComponentId));
		NxFr::Handle<Component> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->InstantiateComponent(Instance, Target);
	}));
	static Command* CmdWorldComponentDuplicate = Command::Create("World.Component.Duplicate"_Sid, "Duplicate Component", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView ComponentId, NxFr::StringView TargetId)
	{
		WorldSystem* World = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Component> Instance = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ComponentId));
		NxFr::Handle<Component> Target = World->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		World->DuplicateComponent(Instance, Target);
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
		WorldManager* Manager = GetManager(Name);
		if (Manager)
		{
			NX_LOG(Warning, System, "World %llu already exist", Name.GetId());
			return Manager->GetWorld();
		}

		Manager = new WorldManager();
		Managers.Append(Name.GetId(), Manager);

		World* World = Manager->CreateWorld(Name);
		World->Initialize();
		World->SetEnabled(true);

		OnWorldChange.Invoke(EventCreatedId, Name);

		NxFr::Handle<GameObject> Root = Manager->CreateGameObject();
		Root->SetName(Name);
		Root->Initialize();
		Root->SetEnabled(true);

		OnWorldObjectChange.Invoke(EventCreatedId, Root->GetWorldId(), Root->GetId());

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

		OnWorldObjectChange.Invoke(EventDestroyedId, Root->GetWorldId(), Root->GetId());

		Root->SetEnabled(false);
		Root->Shutdown();
		Manager->DestroyGameObject(Root);

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

	NxFr::Handle<GameObject> WorldSystem::PackPrefab(NxFr::Handle<GameObject> Original, NxFr::GUID AssetId)
	{
		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(!Belong(Original, PrefabWorldId), Default, "Original should not be a prefab");

		WorldManager* Manager = GetManager(PrefabWorldId);
		NxFr::Handle<GameObject> Parent = Manager->GetWorld()->GetRoot();

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<GameObject> Instance = Manager->DuplicateGameObject(Original, Parent, WorldManager::ReferenceMode::Keep);
		Instance->Clone(Original.GetRedirectedPointer());

		Original->AssetId = AssetId;
		Instance->AssetId = AssetId;

		return Instance;
	}

	void WorldSystem::UnpackPrefab(NxFr::Handle<GameObject> Original)
	{
		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(!Belong(Original, PrefabWorldId), Default, "Original should not be a prefab");

		Original->AssetId = 0;
	}

	YAML::Node WorldSystem::SerializePrefab(NxFr::Handle<GameObject> Instance)
	{
		NX_ASSERT(Instance, Default, "Instance should be valid");
		NX_ASSERT(Belong(Instance, PrefabWorldId), Default, "Instance should be a prefab");

		return Instance->Serialize();
	}

	NxFr::Handle<GameObject> WorldSystem::DeserializePrefab(YAML::Node Node)
	{
		WorldManager* Manager = GetManager(PrefabWorldId);
		NxFr::Handle<GameObject> Parent = Manager->GetWorld()->GetRoot();

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<GameObject> Instance = Manager->CreateGameObject(Node, Parent);
		Instance->Deserialize(Node);

		return Instance;
	}

	void WorldSystem::UnloadPrefab(NxFr::Handle<GameObject> Instance)
	{
		NX_ASSERT(Instance, Default, "Instance should be valid");
		NX_ASSERT(Belong(Instance, PrefabWorldId), Default, "Instance should be a prefab");

		WorldManager* Manager = GetManager(PrefabWorldId);

		Instance->Unload();
		Manager->DestroyGameObject(Instance);
	}

	NxFr::Handle<GameObject> WorldSystem::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		if (Parent)
		{
			WorldId = Parent->GetWorldId();
		}

		NX_ASSERT(WorldId != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		if (!Parent)
		{
			Parent = Manager->GetWorld()->GetRoot();
		}

		NxFr::Handle<GameObject> Instance = Manager->CreateGameObject(Parent);
		Instance->SetName(Name);
		Instance->Initialize();
		Instance->SetEnabled(true);

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetWorldId(), Instance->GetId());

		return Instance;
	}

	NxFr::Handle<GameObject> WorldSystem::InstantiateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		if (Parent)
		{
			WorldId = Parent->GetWorldId();
		}

		NX_ASSERT(WorldId != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Original != Manager->GetWorld()->GetRoot(), Default, "Can't duplicate root object");

		if (!Parent)
		{
			Parent = Manager->GetWorld()->GetRoot();
		}

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<GameObject> Instance = Manager->DuplicateGameObject(Original, Parent, WorldManager::ReferenceMode::Resolve);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetWorldId(), Instance->GetId());

		return Instance;
	}

	NxFr::Handle<GameObject> WorldSystem::DuplicateGameObject(NxFr::Handle<const GameObject> Original, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		if (Parent)
		{
			WorldId = Parent->GetWorldId();
		}

		NX_ASSERT(WorldId != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Original != Manager->GetWorld()->GetRoot(), Default, "Can't duplicate root object");

		if (!Parent)
		{
			Parent = Original->GetParent();
		}
		if (!Parent)
		{
			Parent = Manager->GetWorld()->GetRoot();
		}

		NxFr::Handle<GameObject> Instance = Manager->DuplicateGameObject(Original, Parent, WorldManager::ReferenceMode::Ignore);
		Instance->Clone(Original.GetRedirectedPointer());
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

		NX_ASSERT(Instance->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetWorldId(), Instance->GetId());

		WorldManager* Manager = GetManager(Instance->GetWorldId());

		NX_ASSERT(Instance != Manager->GetWorld()->GetRoot(), Default, "Can't destroy root object");

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyGameObject(Instance);
	}

	void WorldSystem::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index)
	{
		NX_ASSERT(Instance->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

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

	NxFr::Handle<Behaviour> WorldSystem::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Target->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

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

	NxFr::Handle<Behaviour> WorldSystem::InstantiateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Target->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", Target->GetWorldId());
			return NxFr::Handle<Behaviour>();
		}

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<Behaviour> Instance = Manager->DuplicateBehaviour(Original, Target);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	NxFr::Handle<Behaviour> WorldSystem::DuplicateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Target->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", Target->GetWorldId());
			return NxFr::Handle<Behaviour>();
		}

		NxFr::Handle<Behaviour> Instance = Manager->DuplicateBehaviour(Original, Target);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NX_ASSERT(Instance->GetGameObject()->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

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
		NX_ASSERT(Target->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

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

	NxFr::Handle<Component> WorldSystem::InstantiateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Target->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", Target->GetWorldId());
			return NxFr::Handle<Component>();
		}

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<Component> Instance = Manager->DuplicateComponent(Original, Target);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	NxFr::Handle<Component> WorldSystem::DuplicateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT(Target, Default, "Target should be valid");
		NX_ASSERT(Original, Default, "Original should be valid");
		NX_ASSERT(Target->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", Target->GetWorldId());
			return NxFr::Handle<Component>();
		}

		NxFr::Handle<Component> Instance = Manager->DuplicateComponent(Original, Target);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		NX_ASSERT(Instance->GetGameObject()->GetWorldId() != PrefabWorldId, Default, "Can't be use on the prefab world");

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
		NX_ASSERT(Instance, Default, "Instance should be valid");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return false;
		}

		return Manager->Belong(Instance);
	}

	WorldObjectType WorldSystem::GetType(NxFr::Handle<Object> Instance, NxFr::GUID WorldId)
	{
		NX_ASSERT(Instance, Default, "Instance should be valid");

		if (WorldId != Object::NullId)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return WorldObjectType::None;
			}

			return Manager->GetType(Instance);
		}
		else
		{
			WorldObjectType Result = WorldObjectType::None;

			for (auto [Id, Manager] : Managers)
			{
				Result = Manager->GetType(Instance);
				if (Result != WorldObjectType::None)
				{
					break;
				}
			}

			return Result;
		}
	}

	NxFr::Handle<Object> WorldSystem::GetObject(NxFr::GUID ObjectId, NxFr::GUID WorldId)
	{
		if (WorldId != Object::NullId)
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

	NxFr::Array<NxFr::Handle<Object>> WorldSystem::GetObjects(WorldObjectType Type, NxFr::GUID WorldId)
	{
		if (WorldId != Object::NullId)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return NxFr::Array<NxFr::Handle<Object>>();
			}

			return Manager->GetObjects(Type);
		}
		else
		{
			NxFr::List<NxFr::Handle<Object>> Result;

			for (auto [Id, Manager] : Managers)
			{
				Result.AppendRange(Manager->GetObjects(Type));
			}

			return NxFr::ContainerUtility::ToArray<NxFr::Handle<Object>>(Result);
		}
	}

	NxFr::Array<NxFr::GUID> WorldSystem::Find(NxFr::StringView Query, WorldObjectType Type, NxFr::GUID WorldId)
	{
		if (WorldId != Object::NullId)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return NxFr::Array<NxFr::GUID>();
			}

			return Manager->Find(Query, Type);
		}
		else
		{
			NxFr::List<NxFr::GUID> Result;

			for (auto [Id, Manager] : Managers)
			{
				Result.AppendRange(Manager->Find(Query, Type));
			}

			return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
		}
	}

	NxFr::Array<NxFr::GUID> WorldSystem::FindGameObjects(NxFr::StringView Query, NxFr::GUID WorldId)
	{
		if (WorldId != Object::NullId)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return NxFr::Array<NxFr::GUID>();
			}

			return Manager->FindGameObjects(Query);
		}
		else
		{
			NxFr::List<NxFr::GUID> Result;

			for (auto [Id, Manager] : Managers)
			{
				Result.AppendRange(Manager->FindGameObjects(Query));
			}

			return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
		}
	}

	NxFr::Array<NxFr::GUID> WorldSystem::GetDependencies(NxFr::GUID Id, bool Recursive)
	{
		NxFr::Set<NxFr::GUID> Dependencies;
		GetDependencies(Id, Recursive, Dependencies);
		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Dependencies);
	}

	void WorldSystem::GetDependencies(NxFr::GUID Id, bool Recursive, NxFr::Set<NxFr::GUID>& Result)
	{
		NxFr::Handle<Object> Instance = GetObject(Id);
		NxFr::Array<NxFr::GUID> Dependencies = Instance->GetDependencies();

		for (auto& Dependency : Dependencies)
		{
			if (Dependency == 0 || Result.TryGet(Dependency))
			{
				continue;
			}

			Result.Append(Dependency);
			if (Recursive)
			{
				GetDependencies(Dependency, Recursive, Result);
			}
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
		CreateWorld(PrefabWorldId);

		GetWorld(PrefabWorldId)->SetTickable(false);
	}

	void WorldSystem::OnShutdown()
	{
		System::OnShutdown();

		DestroyWorld(PrefabWorldId);
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
