#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/World/World/World.h"
#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"
#include "NexusEngine/Systems/World/Assets/Scene.h"
#include "NexusEngine/Systems/World/Assets/Prefab.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

namespace NxEn
{
	class WorldSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, WorldSystem)

		inline static const NxFr::StringId DummyId = "Dummy"_Sid;
		inline static const NxFr::StringId WorldId = "World"_Sid;

		inline static const NxFr::StringId AppendedId = "Added"_Sid;
		inline static const NxFr::StringId RemovedId = "Removed"_Sid;

		NEXUS_ENGINE_API WorldSystem();
		NEXUS_ENGINE_API ~WorldSystem();

		NEXUS_ENGINE_API World* CreateWorld(NxFr::GUID WorldId, NxFr::StringView Name = "", bool References = false);
		NEXUS_ENGINE_API World* GetWorld(NxFr::GUID WorldId = WorldId);
		NEXUS_ENGINE_API NxFr::Array<NxFr::GUID> GetWorlds();
		NEXUS_ENGINE_API void DestroyWorld(NxFr::GUID WorldId);

		NEXUS_ENGINE_API Scene* CreateScene(NxFr::StringView Path, NxFr::GUID WorldId = WorldId);
		NEXUS_ENGINE_API void SaveScene(NxFr::GUID SceneId);
		NEXUS_ENGINE_API void SaveScenes();
		NEXUS_ENGINE_API Scene* LoadScene(NxFr::GUID SceneId, NxFr::GUID WorldId = WorldId);
		NEXUS_ENGINE_API Scene* LoadSceneSingle(NxFr::GUID SceneId, NxFr::GUID WorldId = WorldId);
		NEXUS_ENGINE_API void UnloadScene(NxFr::GUID SceneId);
		NEXUS_ENGINE_API NxFr::GUID IsSceneLoaded(NxFr::GUID SceneId);
		NEXUS_ENGINE_API NxFr::Array<NxFr::GUID> GetScenes(NxFr::GUID WorldId = 0);

		NEXUS_ENGINE_API Prefab* CreatePrefab(NxFr::Handle<GameObject> Target, NxFr::StringView Path);
		NEXUS_ENGINE_API void SavePrefab(NxFr::Handle<GameObject> Target);
		NEXUS_ENGINE_API Prefab* LoadPrefab(NxFr::GUID PrefabId);
		NEXUS_ENGINE_API void UnloadPrefab(NxFr::GUID PrefabId);
		NEXUS_ENGINE_API void UnpackPrefab(NxFr::Handle<GameObject> Target);
		NEXUS_ENGINE_API NxFr::Handle<GameObject> InstantiatePrefab(Prefab* Instance, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = WorldId);

		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnWorldEvent() { return OnWorldEvent; }
		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnGameObjectEvent() { return OnGameObjectEvent; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnWorldEvent;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnGameObjectEvent;

		NxFr::Dictionary<NxFr::GUID, World*> Worlds;
		NxFr::Dictionary<NxFr::GUID, NxFr::GUID> Scenes;
	};
}
