#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldSystem.h"

namespace NxEn
{
	const static Command CmdWorldSceneCreate = Command::Create("World.Scene.Create"_Sid, "Create scene", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		Application::GetSystem<WorldSystem>()->CreateScene(Path);
	}));

	const static Command CmdWorldSceneSave = Command::Create("World.Scene.Save"_Sid, "Save scene", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* System = Application::GetSystem<AssetsSystem>();
		NxFr::GUID Id = System->PathToId(Path);
		Application::GetSystem<WorldSystem>()->SaveScene(Id);
	}));

	const static Command CmdWorldSceneLoad = Command::Create("World.Scene.Load"_Sid, "Load scene into the Main world", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* System = Application::GetSystem<AssetsSystem>();
		NxFr::GUID Id = System->PathToId(Path);
		Application::GetSystem<WorldSystem>()->LoadScene(Id);
	}));

	const static Command CmdWorldSceneUnload = Command::Create("World.Scene.Unload"_Sid, "Unload scene", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* System = Application::GetSystem<AssetsSystem>();
		NxFr::GUID Id = System->PathToId(Path);
		Application::GetSystem<WorldSystem>()->UnloadScene(Id);
	}));

	const static Command CmdWorldInstantiate = Command::Create("World.Prefab.Instantiate"_Sid, "Instantiate into the Main world", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		WorldSystem* Worlds = Application::GetSystem<WorldSystem>();

		NxFr::GUID Id = Assets->PathToId(Path);
		Prefab* Instance = Worlds->LoadPrefab(Id);
		Worlds->InstantiatePrefab(Instance);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(WorldSystem)

	WorldSystem::WorldSystem()
		: OnWorldEvent(), OnGameObjectEvent(), Worlds()
	{
	}

	WorldSystem::~WorldSystem()
	{
	}

	World* WorldSystem::CreateWorld(NxFr::StringId Name, bool References)
	{
		NxFr::GUID WorldId = Name;
		if (Worlds.ContainsKey(WorldId))
		{
			NEXUS_LOG(Warning, System, "World %d already exist", WorldId);
			return GetWorld(WorldId);
		}

		World* Instance = new World(Name, References);
		Instance->Initialize();
		Instance->SetEnabled(true);

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

		NxFr::Array<NxFr::GUID> LoadedScene = GetScenes(WorldId);
		for (auto& SceneId : LoadedScene)
		{
			UnloadScene(SceneId);
		}

		World* Instance = Worlds[WorldId];

		Instance->SetEnabled(false);
		Instance->Shutdown();
		delete Instance;

		Worlds.Remove(WorldId);
	}

	Scene* WorldSystem::CreateScene(NxFr::StringView Path, NxFr::GUID WorldId)
	{
		World* WorldInstance = GetWorld(WorldId);
		WorldObjectFactoryContext Context(WorldInstance);

		Scene* SceneInstance = Application::GetSystem<AssetsSystem>()->Create<Scene>(Path, "scene");

		WorldInstance->AttachGameObject(SceneInstance->GetRoot(), WorldInstance->GetRootGameObject());
		SceneInstance->SetDirty();

		Scenes.Append(SceneInstance->GetId(), WorldId);
		GetOnSceneEvent().Invoke(AppendedId, WorldId, SceneInstance->GetId());
		GetOnGameObjectEvent().Invoke(AppendedId, WorldId, SceneInstance->GetRoot()->GetId());

		return SceneInstance;
	}

	void WorldSystem::SaveScene(NxFr::GUID SceneId)
	{
		if (!IsSceneLoaded(SceneId))
		{
			NEXUS_LOG(Error, System, "Scene %d is not loaded", SceneId);
			return;
		}

		Application::GetSystem<AssetsSystem>()->Save(SceneId, true);
	}

	void WorldSystem::SaveScenes()
	{
		for (auto& [IdScene, IdWorld] : Scenes)
		{
			SaveScene(IdScene);
		}
	}

	Scene* WorldSystem::LoadScene(NxFr::GUID SceneId, NxFr::GUID WorldId)
	{
		if (IsSceneLoaded(SceneId))
		{
			NEXUS_LOG(Error, System, "Scene %d is already loaded", SceneId);
			return nullptr;
		}

		World* WorldInstance = GetWorld(WorldId);
		WorldObjectFactoryContext Context(WorldInstance);
		WorldObjectReferences References;

		Scene* SceneInstance = Application::GetSystem<AssetsSystem>()->Load<Scene>(SceneId);

		WorldInstance->AttachGameObject(SceneInstance->GetRoot(), WorldInstance->GetRootGameObject());
		SceneInstance->SetDirty();

		Scenes.Append(SceneId, WorldId);
		GetOnSceneEvent().Invoke(AppendedId, WorldId, SceneId);
		GetOnGameObjectEvent().Invoke(AppendedId, WorldId, SceneInstance->GetRoot()->GetId());

		return SceneInstance;
	}

	Scene* WorldSystem::LoadSceneSingle(NxFr::GUID SceneId, NxFr::GUID WorldId)
	{
		NxFr::Array<NxFr::GUID> Ids = GetScenes(WorldId);
		for (auto Id : Ids)
		{
			UnloadScene(Id);
		}

		return LoadScene(SceneId);
	}

	void WorldSystem::UnloadScene(NxFr::GUID SceneId)
	{
		NxFr::GUID WorldId = IsSceneLoaded(SceneId);
		if (!WorldId)
		{
			NEXUS_LOG(Error, System, "Scene %d is not loaded", SceneId, WorldId);
			return;
		}

		World* WorldInstance = GetWorld(WorldId);
		WorldObjectFactoryContext Context(WorldInstance);

		Scene* SceneInstance = Application::GetSystem<AssetsSystem>()->GetAsset<Scene>(SceneId);

		Scenes.Remove(SceneId);
		GetOnSceneEvent().Invoke(RemovedId, WorldId, SceneId);
		GetOnGameObjectEvent().Invoke(RemovedId, WorldId, SceneInstance->GetRoot()->GetId());

		WorldInstance->DetachGameObject(SceneInstance->GetRoot());
		Application::GetSystem<AssetsSystem>()->Unload(SceneId);
	}

	NxFr::GUID WorldSystem::IsSceneLoaded(NxFr::GUID SceneId)
	{
		bool Loaded = Application::GetSystem<AssetsSystem>()->IsLoaded(SceneId);
		NxFr::GUID* WorldId = Scenes.TryGet(SceneId);
		return Loaded && WorldId ? *WorldId : 0;
	}

	NxFr::Array<NxFr::GUID> WorldSystem::GetScenes(NxFr::GUID WorldId)
	{
		NxFr::List<NxFr::GUID> Result(Scenes.GetCount());
		uint64 Index = 0;

		for (auto& [IdScene, IdWorld] : Scenes)
		{
			if (WorldId == 0 || WorldId == IdWorld)
			{
				Result.Append(IdScene);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Result);
	}

	Prefab* WorldSystem::CreatePrefab(NxFr::Handle<GameObject> Target, NxFr::StringView Path)
	{
		World* WorldInstance = GetWorld(DummyId);
		WorldObjectFactoryContext Context(WorldInstance);
		WorldObjectReferences References;

		Prefab* PrefabInstance = Application::GetSystem<AssetsSystem>()->Create<Prefab>(Path, "prefab");
		PrefabInstance->SetRoot(Target);
		return PrefabInstance;
	}

	void WorldSystem::SavePrefab(NxFr::Handle<GameObject> Target)
	{
		World* WorldInstance = GetWorld(DummyId);
		WorldObjectFactoryContext Context(WorldInstance);
		WorldObjectReferences References;

		Prefab* PrefabInstance = Application::GetSystem<AssetsSystem>()->GetAsset<Prefab>(Target->GetReferenceId());
		PrefabInstance->SetRoot(Target);
	}

	Prefab* WorldSystem::LoadPrefab(NxFr::GUID PrefabId)
	{
		World* WorldInstance = GetWorld(DummyId);
		WorldObjectFactoryContext Context(WorldInstance);
		WorldObjectReferences References;

		return Application::GetSystem<AssetsSystem>()->Load<Prefab>(PrefabId);
	}

	void WorldSystem::UnloadPrefab(NxFr::GUID PrefabId)
	{
		World* WorldInstance = GetWorld(DummyId);
		WorldObjectFactoryContext Context(WorldInstance);
		WorldObjectReferences References;

		Application::GetSystem<AssetsSystem>()->Unload(PrefabId);
	}

	void WorldSystem::UnpackPrefab(NxFr::Handle<GameObject> Target)
	{
		if (!Target->GetReferenceId() || Target->IsRoot())
		{
			return;
		}

		Target->ReferenceId = 0;
	}

	NxFr::Handle<GameObject> WorldSystem::InstantiatePrefab(Prefab* Instance, NxFr::Handle<GameObject> Parent, NxFr::GUID WorldId)
	{
		World* WorldInstance = GetWorld(WorldId);
		WorldObjectFactoryContext Context(WorldInstance);
		WorldObjectReferences References;

		return WorldInstance->DuplicateGameObject(Instance->GetRoot(), Parent, true);
	}

	void WorldSystem::OnInitialize()
	{
		System::OnInitialize();

		CreateWorld(DummyId, true);
		CreateWorld(WorldId);

		GetWorld(DummyId)->SetTickable(false);
	}

	void WorldSystem::OnShutdown()
	{
		DestroyWorld(WorldId);
		DestroyWorld(DummyId);

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
