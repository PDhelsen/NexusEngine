#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"
#include "NexusEngine/Systems/World/WorldManager.h"

namespace NxEn
{
	static Command* CmdWorldWorldCreate = Command::Create("World.World.Create"_Sid, "Create World", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Name)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		Worlds->CreateWorld(Name);
	}));
	static Command* CmdWorldWorldDestroy = Command::Create("World.World.Destroy"_Sid, "Destroy World", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView WorldId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		Worlds->DestroyWorld(!WorldId.IsEmpty() ? NxFr::StringId(WorldId) : WorldSystem::MainWorldId);
	}));
	static Command* CmdWorldScenePack = Command::Create("World.Scene.Pack "_Sid, "Pack  scene", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView SceneId, NxFr::StringView WorldId)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();

		Scene* SceneInstance = Assets->Load<Scene>(NxFr::StringUtility::FromString<NxFr::GUID>(SceneId));
		Worlds->PackScene(SceneInstance, !WorldId.IsEmpty() ? NxFr::StringId(WorldId) : WorldSystem::MainWorldId);
	}));
	static Command* CmdWorldSceneInstantiate = Command::Create("World.Scene.Instantiate"_Sid, "Instantiate Scene", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView SceneId, NxFr::StringView WorldId)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();

		Scene* SceneInstance = Assets->Load<Scene>(NxFr::StringUtility::FromString<NxFr::GUID>(SceneId));
		Worlds->InstantiateScene(SceneInstance, !WorldId.IsEmpty() ? NxFr::StringId(WorldId) : WorldSystem::MainWorldId);
	}));
	static Command* CmdWorldSceneDestroy = Command::Create("World.Scene.Destroy"_Sid, "Destroy Scene", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView SceneId, NxFr::StringView WorldId)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();

		Scene* SceneInstance = Assets->Load<Scene>(NxFr::StringUtility::FromString<NxFr::GUID>(SceneId));
		Worlds->DestroyScene(SceneInstance, !WorldId.IsEmpty() ? NxFr::StringId(WorldId) : WorldSystem::MainWorldId);
	}));
	static Command* CmdWorldPrefabPack = Command::Create("World.Prefab.Pack"_Sid, "Pack Prefab", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView PrefabId, NxFr::StringView GameObjectId)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();

		Prefab* PrefabInstance = Assets->Load<Prefab>(NxFr::StringUtility::FromString<NxFr::GUID>(PrefabId));
		NxFr::Handle<GameObject> GameObjectInstance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		Worlds->PackPrefab(PrefabInstance, GameObjectInstance);
	}));
	static Command* CmdWorldPrefabUnpack = Command::Create("World.Prefab.Unpack"_Sid, "Unpack Prefab", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView GameObjectId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> GameObjectInstance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		Worlds->UnpackPrefab(GameObjectInstance);
	}));
	static Command* CmdWorldGameObjectCreate = Command::Create("World.GameObject.Create"_Sid, "Create GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Name, NxFr::StringView WorldId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		Worlds->CreateGameObject(Name, NxFr::Handle<GameObject>(), !WorldId.IsEmpty() ? NxFr::StringId(WorldId) : WorldSystem::MainWorldId);
	}));
	static Command* CmdWorldGameObjectInstantiate = Command::Create("World.GameObject.Instantiate"_Sid, "Instantiate GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView TemplateId, NxFr::StringView WorldId)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();

		NxFr::GUID PrefabOrGameObjectId = NxFr::StringUtility::FromString<NxFr::GUID>(TemplateId);
		NxFr::StringId WorldOrDefaultId = !WorldId.IsEmpty() ? NxFr::StringId(WorldId) : WorldSystem::MainWorldId;
		if (Assets->IsTracked(PrefabOrGameObjectId))
		{
			Prefab* PrefabInstance = Assets->Load<Prefab>(PrefabOrGameObjectId);
			Worlds->InstantiateGameObject(PrefabInstance->GetRoot(), NxFr::Handle<GameObject>(), WorldOrDefaultId);
		}
		else
		{
			NxFr::Handle<GameObject> GameObjectInstance = Worlds->GetObject(PrefabOrGameObjectId);
			Worlds->InstantiateGameObject(GameObjectInstance, NxFr::Handle<GameObject>(), WorldOrDefaultId);
		}
	}));
	static Command* CmdWorldGameObjectDuplicate = Command::Create("World.GameObject.Duplicate"_Sid, "Duplicate GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView GameObjectId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		Worlds->DuplicateGameObject(Instance);
	}));
	static Command* CmdWorldGameObjectDestroy = Command::Create("World.GameObject.Destroy"_Sid, "Destroy GameObject", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView GameObjectId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		Worlds->DestroyGameObject(Instance);
	}));
	static Command* CmdWorldGameObjectAttach = Command::Create("World.GameObject.Attach"_Sid, "Attach GameObject", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView GameObjectId, NxFr::StringView ParentId, NxFr::StringView Index)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(GameObjectId));
		NxFr::Handle<GameObject> Parent = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ParentId));
		Worlds->AttachGameObject(Instance, Parent, NxFr::StringUtility::FromString<uint64>(Index));
	}));
	static Command* CmdWorldBehaviourCreate = Command::Create("World.Behaviour.Create"_Sid, "Create Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView TargetId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		Worlds->CreateBehaviour(Type, Target);
	}));
	static Command* CmdWorldBehaviourInstantiate = Command::Create("World.Behaviour.Instantiate"_Sid, "Instantiate Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView BehaviourId, NxFr::StringView TargetId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Behaviour> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(BehaviourId));
		NxFr::Handle<Behaviour> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		Worlds->InstantiateBehaviour(Instance, Target);
	}));
	static Command* CmdWorldBehaviourDuplicate = Command::Create("World.Behaviour.Duplicate"_Sid, "Duplicate Behaviour", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView BehaviourId, NxFr::StringView TargetId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Behaviour> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(BehaviourId));
		NxFr::Handle<Behaviour> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		Worlds->DuplicateBehaviour(Instance, Target);
	}));
	static Command* CmdWorldBehaviourDestroy = Command::Create("World.Behaviour.Destroy"_Sid, "Destroy Behaviour", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView BehaviourId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Behaviour> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(BehaviourId));
		Worlds->DestroyBehaviour(Instance);
	}));
	static Command* CmdWorldComponentCreate = Command::Create("World.Component.Create"_Sid, "Create Component", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView TargetId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<GameObject> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		Worlds->CreateComponent(Type, Target);
	}));
	static Command* CmdWorldComponentInstantiate = Command::Create("World.Component.Instantiate"_Sid, "Instantiate Component", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView ComponentId, NxFr::StringView TargetId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Component> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ComponentId));
		NxFr::Handle<Component> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		Worlds->InstantiateComponent(Instance, Target);
	}));
	static Command* CmdWorldComponentDuplicate = Command::Create("World.Component.Duplicate"_Sid, "Duplicate Component", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView ComponentId, NxFr::StringView TargetId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Component> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ComponentId));
		NxFr::Handle<Component> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(TargetId));
		Worlds->DuplicateComponent(Instance, Target);
	}));
	static Command* CmdWorldComponentDestroy = Command::Create("World.Component.Destroy"_Sid, "Destroy Component", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView ComponentId)
	{
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();
		NxFr::Handle<Component> Instance = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(ComponentId));
		Worlds->DestroyComponent(Instance);
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

		NxFr::Handle<GameObject> Root = Manager->CreateGameObject();
		Root->SetName(Name);
		Root->Initialize();
		Root->SetEnabled(true);

		OnWorldChange.Invoke(EventCreatedId, Name, false);
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
		OnWorldChange.Invoke(EventDestroyedId, WorldId, false);

		Root->SetEnabled(false);
		Root->Shutdown();
		Manager->DestroyGameObject(Root);

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

	NxFr::Array<NxFr::GUID> WorldSystem::GetWorlds(bool IncludeTemplateWorld)
	{
		uint64 Index = 0;
		uint64 Count = Managers.GetCount();
		if (Managers.TryGet(TemplateWorldId) && !IncludeTemplateWorld)
		{
			Count -= 1;
		}
		NxFr::Array<NxFr::GUID> Result = Count;

		for (auto& [Id, Manager] : Managers)
		{
			if (!IncludeTemplateWorld && Id == TemplateWorldId)
			{
				continue;
			}

			if (Result.IsValidIndex(Index))
			{
				Result[Index++] = Id;
			}
		}

		return Result;
	}

	bool WorldSystem::IsWorld(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance)
		{
			return false;
		}

		WorldManager* Manager = GetManager(Instance->GetWorldId());
		return Instance == Manager->GetWorld()->GetRoot();
	}

	NxFr::Handle<GameObject> WorldSystem::InstantiateScene(Scene* Instance, NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(Instance, NxFr::Handle<GameObject>(), Default, "Instance should be valid");
		NX_ASSERT_RETURN(WorldId != TemplateWorldId, NxFr::Handle<GameObject>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		NxFr::Handle<GameObject> Parent = Manager->GetWorld()->GetRoot();
		NxFr::Handle<GameObject> Original = Instance->GetRoot();

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<GameObject> Root = Manager->DuplicateGameObject(Original, Parent, WorldManager::ReferenceMode::Resolve);
		Root->Clone(Original.GetRedirectedPointer());
		Root->Initialize();
		Root->UpdateHierarchy();

		Manager->RegisterScene(Instance->GetId(), Root);

		OnWorldChange.Invoke(EventCreatedId, Instance->GetId(), true);
		OnWorldObjectChange.Invoke(EventCreatedId, Root->GetWorldId(), Root->GetId());

		return Root;
	}

	NxFr::Handle<GameObject> WorldSystem::InstantiateSceneSingle(Scene* Instance, NxFr::GUID WorldId)
	{
		DestroyScenes(WorldId);
		return InstantiateScene(Instance, WorldId);
	}

	void WorldSystem::DestroyScene(Scene* Instance, NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(Instance, , Default, "Instance should be valid");
		NX_ASSERT_RETURN(WorldId != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return;
		}

		NxFr::Handle<GameObject> Root = Manager->GetScene(Instance->GetId());
		NX_ASSERT_RETURN(Instance, , Default, "Scene is not loaded");

		OnWorldObjectChange.Invoke(EventDestroyedId, Root->GetWorldId(), Root->GetId());
		OnWorldChange.Invoke(EventDestroyedId, Instance->GetId(), true);

		Manager->UnregisterScene(Instance->GetId());

		Root->SetEnabled(false);
		Root->Shutdown();
		Manager->DestroyGameObject(Root);
	}

	void WorldSystem::DestroyScenes(NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(WorldId != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return;
		}

		NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>> Scenes = Manager->GetScenes();
		for (auto [Id, Original] : Scenes)
		{
			AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
			Scene* SceneInstance = Assets->GetAsset<Scene>(Id);
			DestroyScene(SceneInstance, WorldId);
		}
	}

	void WorldSystem::PackScene(Scene* Instance, NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(Instance, , Default, "Instance should be valid");
		NX_ASSERT_RETURN(WorldId != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return;
		}

		NxFr::Handle<GameObject> Original = Manager->GetScene(Instance->GetId());
		NX_ASSERT_RETURN(Original, , Default, "Scene is not loaded");

		NxFr::Handle<GameObject> Root = TemplateGameObject(Original, Instance->GetId());
		Instance->SetRoot(Root);
	}

	void WorldSystem::PackScenes(NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(WorldId != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return;
		}

		NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>> Scenes = Manager->GetScenes();
		for (auto [Id, Original] : Scenes)
		{
			AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
			Scene* SceneInstance = Assets->GetAsset<Scene>(Id);
			PackScene(SceneInstance, WorldId);
		}
	}

	bool WorldSystem::IsSceneInstantiated(NxFr::GUID SceneId, NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(WorldId != TemplateWorldId, false, Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return false;
		}

		NxFr::Handle<GameObject> Root = Manager->GetScene(SceneId);
		return Root.GetRedirectedPointer();
	}

	bool WorldSystem::IsScene(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance || !Instance->GetTemplateId())
		{
			return false;
		}

		AssetMetadata* Metadata = Application::GetSystem<AssetsSystem>()->GetMetadata(Instance->GetTemplateId());
		return Metadata->GetType() == Scene::GetClassType();
	}

	void WorldSystem::PackPrefab(Prefab* Instance, NxFr::Handle<GameObject> Original)
	{
		NX_ASSERT_RETURN(Instance, , Default, "Instance should be valid");

		NxFr::Handle<GameObject> Root = TemplateGameObject(Original, Instance->GetId());
		NX_ASSERT_RETURN(Root, , Default, "Root should be valid");

		Instance->SetRoot(Root);
	}

	void WorldSystem::UnpackPrefab(NxFr::Handle<GameObject> Original)
	{
		UntemplateGameObject(Original);
	}

	bool WorldSystem::IsPrefab(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance || !Instance->GetTemplateId())
		{
			return false;
		}

		AssetMetadata* Metadata = Application::GetSystem<AssetsSystem>()->GetMetadata(Instance->GetTemplateId());
		return Metadata->GetType() == Prefab::GetClassType();
	}

	NxFr::Handle<GameObject> WorldSystem::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		if (Parent)
		{
			WorldId = Parent->GetWorldId();
		}

		NX_ASSERT_RETURN(WorldId != TemplateWorldId, NxFr::Handle<GameObject>(), Default, "Can't be use on the prefab world");

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

		NX_ASSERT_RETURN(WorldId != TemplateWorldId, NxFr::Handle<GameObject>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(WorldId);
		if (!Manager)
		{
			NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
			return NxFr::Handle<GameObject>();
		}

		NX_ASSERT_RETURN(Original, NxFr::Handle<GameObject>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(!IsWorld(Original), NxFr::Handle<GameObject>(), Default, "Can't duplicate root object");
		NX_ASSERT_RETURN(!IsScene(Original), NxFr::Handle<GameObject>(), Default, "Can't duplicate scene root object");

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

	NxFr::Handle<GameObject> WorldSystem::DuplicateGameObject(NxFr::Handle<const GameObject> Original, NxFr::Handle<GameObject> Parent)
	{
		NX_ASSERT_RETURN(Original->GetWorldId() != TemplateWorldId, NxFr::Handle<GameObject>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Original->GetWorldId());

		NX_ASSERT_RETURN(Original, NxFr::Handle<GameObject>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(!IsWorld(Original), NxFr::Handle<GameObject>(), Default, "Can't duplicate root object");
		NX_ASSERT_RETURN(!IsScene(Original), NxFr::Handle<GameObject>(), Default, "Can't duplicate scene root object");

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

		NX_ASSERT_RETURN(Instance->GetWorldId() != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Instance->GetWorldId());

		NX_ASSERT_RETURN(!IsWorld(Instance), , Default, "Can't destroy world root object");
		NX_ASSERT_RETURN(!IsScene(Instance), , Default, "Can't destroy scene root object");

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetWorldId(), Instance->GetId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyGameObject(Instance);
	}

	void WorldSystem::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index)
	{
		NX_ASSERT_RETURN(Instance->GetWorldId() != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Instance->GetWorldId());

		NX_ASSERT_RETURN(Instance, , Default, "Instance should be valid");
		NX_ASSERT_RETURN(!IsWorld(Instance), , Default, "Can't attach root object");
		NX_ASSERT_RETURN(!IsScene(Instance), , Default, "Can't attach scene root object");
		NX_ASSERT_RETURN(!Parent || Parent->GetWorldId() == Instance->GetWorldId(), , Default, "Parent should belong to the same world");

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

	YAML::Node WorldSystem::SerializeGameObject(NxFr::Handle<GameObject> Instance)
	{
		NX_ASSERT_RETURN(Instance, YAML::Node(), Default, "Instance should be valid");
		NX_ASSERT_RETURN(Belong(Instance, TemplateWorldId), YAML::Node(), Default, "Instance should be a template");

		return Instance->Serialize();
	}

	NxFr::Handle<GameObject> WorldSystem::DeserializeGameObject(YAML::Node Node)
	{
		WorldManager* Manager = GetManager(TemplateWorldId);
		NxFr::Handle<GameObject> Parent = Manager->GetWorld()->GetRoot();

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<GameObject> Instance = Manager->CreateGameObject(Node, Parent);
		Instance->Deserialize(Node);

		return Instance;
	}

	void WorldSystem::UnloadGameObject(NxFr::Handle<GameObject> Instance)
	{
		NX_ASSERT_RETURN(Instance, , Default, "Instance should be valid");
		NX_ASSERT_RETURN(Belong(Instance, TemplateWorldId), , Default, "Instance should be a template");

		WorldManager* Manager = GetManager(TemplateWorldId);

		NX_ASSERT_RETURN(!IsWorld(Instance), , Default, "Can't unload root object");

		Instance->Unload();
		Manager->DestroyGameObject(Instance);
	}

	NxFr::Handle<GameObject> WorldSystem::TemplateGameObject(NxFr::Handle<GameObject> Original, NxFr::GUID TemplateId)
	{
		NX_ASSERT_RETURN(Original, NxFr::Handle<GameObject>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(!Belong(Original, TemplateWorldId), NxFr::Handle<GameObject>(), Default, "Original should not be a template");

		WorldManager* Manager = GetManager(TemplateWorldId);

		NX_ASSERT_RETURN(!IsWorld(Original), NxFr::Handle<GameObject>(), Default, "Can't template root object");

		NxFr::Handle<GameObject> Parent = Manager->GetWorld()->GetRoot();

		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>::Value IdMap = Manager->GetIdsRemap().PushValue();
		NxFr::Handle<GameObject> Root = Manager->DuplicateGameObject(Original, Parent, WorldManager::ReferenceMode::Keep);
		Root->Clone(Original.GetRedirectedPointer());

		Root->TemplateId = TemplateId;
		Original->TemplateId = TemplateId;

		return Root;
	}

	void WorldSystem::UntemplateGameObject(NxFr::Handle<GameObject> Instance)
	{
		NX_ASSERT_RETURN(Instance, , Default, "Original should be valid");
		NX_ASSERT_RETURN(!Belong(Instance, TemplateWorldId), , Default, "Original should not be a template");

		Instance->TemplateId = 0;
	}

	NxFr::Handle<Behaviour> WorldSystem::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT_RETURN(Target, NxFr::Handle<Behaviour>(), Default, "Target should be valid");
		NX_ASSERT_RETURN(Target->GetWorldId() != TemplateWorldId, NxFr::Handle<Behaviour>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());

		NxFr::Handle<Behaviour> Instance = Manager->CreateBehaviour(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	NxFr::Handle<Behaviour> WorldSystem::InstantiateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT_RETURN(Target, NxFr::Handle<Behaviour>(), Default, "Target should be valid");
		NX_ASSERT_RETURN(Original, NxFr::Handle<Behaviour>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(Target->GetWorldId() != TemplateWorldId, NxFr::Handle<Behaviour>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());

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
		NX_ASSERT_RETURN(Target, NxFr::Handle<Behaviour>(), Default, "Target should be valid");
		NX_ASSERT_RETURN(Original, NxFr::Handle<Behaviour>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(Target->GetWorldId() != TemplateWorldId, NxFr::Handle<Behaviour>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());

		NxFr::Handle<Behaviour> Instance = Manager->DuplicateBehaviour(Original, Target);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		if (!Instance)
		{
			return;
		}

		NX_ASSERT_RETURN(Instance->GetGameObject()->GetWorldId() != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Instance->GetGameObject()->GetWorldId());

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyBehaviour(Instance);
	}

	NxFr::Handle<Component> WorldSystem::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT_RETURN(Target, NxFr::Handle<Component>(), Default, "Target should be valid");
		NX_ASSERT_RETURN(Target->GetWorldId() != TemplateWorldId, NxFr::Handle<Component>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());

		NxFr::Handle<Component> Instance = Manager->CreateComponent(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	NxFr::Handle<Component> WorldSystem::InstantiateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target)
	{
		NX_ASSERT_RETURN(Target, NxFr::Handle<Component>(), Default, "Target should be valid");
		NX_ASSERT_RETURN(Original, NxFr::Handle<Component>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(Target->GetWorldId() != TemplateWorldId, NxFr::Handle<Component>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());

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
		NX_ASSERT_RETURN(Target, NxFr::Handle<Component>(), Default, "Target should be valid");
		NX_ASSERT_RETURN(Original, NxFr::Handle<Component>(), Default, "Original should be valid");
		NX_ASSERT_RETURN(Target->GetWorldId() != TemplateWorldId, NxFr::Handle<Component>(), Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Target->GetWorldId());

		NxFr::Handle<Component> Instance = Manager->DuplicateComponent(Original, Target);
		Instance->Clone(Original.GetRedirectedPointer());
		Instance->Initialize();
		Instance->UpdateHierarchy();

		OnWorldObjectChange.Invoke(EventCreatedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		return Instance;
	}

	void WorldSystem::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		if (!Instance)
		{
			return;
		}

		NX_ASSERT_RETURN(Instance->GetGameObject()->GetWorldId() != TemplateWorldId, , Default, "Can't be use on the prefab world");

		WorldManager* Manager = GetManager(Instance->GetGameObject()->GetWorldId());

		OnWorldObjectChange.Invoke(EventDestroyedId, Instance->GetGameObject()->GetWorldId(), Instance->GetId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Manager->DestroyComponent(Instance);
	}

	bool WorldSystem::Belong(NxFr::Handle<Object> Instance, NxFr::GUID WorldId)
	{
		NX_ASSERT_RETURN(Instance, false, Default, "Instance should be valid");

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
		NX_ASSERT_RETURN(Instance, WorldObjectType::None, Default, "Instance should be valid");

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

	NxFr::Handle<GameObject> WorldSystem::GetScene(NxFr::GUID SceneId, NxFr::GUID WorldId)
	{
		if (WorldId != Object::NullId)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return NxFr::Handle<GameObject>();
			}

			return Manager->GetScene(SceneId);
		}
		else
		{
			NxFr::Handle<Object> Result;

			for (auto [Id, Manager] : Managers)
			{
				Result = Manager->GetScene(SceneId);
				if (Result)
				{
					break;
				}
			}

			return Result;
		}
	}

	NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>> WorldSystem::GetScenes(NxFr::GUID WorldId)
	{
		if (WorldId != Object::NullId)
		{
			WorldManager* Manager = GetManager(WorldId);
			if (!Manager)
			{
				NX_LOG(Warning, System, "World %llu doesn't exist", WorldId);
				return NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>>();
			}

			return Manager->GetScenes();
		}
		else
		{
			NxFr::Dictionary<NxFr::GUID, NxFr::Handle<GameObject>> Result;

			for (auto [Id, Manager] : Managers)
			{
				Result.TryAppendRange(Manager->GetScenes());
			}

			return Result;
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
		CreateWorld(TemplateWorldId);

		GetWorld(TemplateWorldId)->SetTickable(false);
	}

	void WorldSystem::OnShutdown()
	{
		System::OnShutdown();

		DestroyWorld(TemplateWorldId);
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
