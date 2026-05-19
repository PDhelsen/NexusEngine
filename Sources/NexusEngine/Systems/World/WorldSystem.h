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
		NX_OBJECT_DECLARATION(NX_ENGINE_API, WorldSystem)

		inline static const NxFr::StringId DummyId = "Dummy"_Sid;
		inline static const NxFr::StringId WorldId = "World"_Sid;

		inline static const NxFr::StringId AppendedId = "Added"_Sid;
		inline static const NxFr::StringId RemovedId = "Removed"_Sid;

		NX_ENGINE_API WorldSystem();
		NX_ENGINE_API ~WorldSystem();

		NX_ENGINE_API World* CreateWorld(NxFr::StringId Name, bool References = false);
		NX_ENGINE_API World* GetWorld(NxFr::GUID WorldId = WorldId);
		NX_ENGINE_API NxFr::Array<NxFr::GUID> GetWorlds();
		NX_ENGINE_API void DestroyWorld(NxFr::GUID WorldId);

		NX_ENGINE_API Scene* CreateScene(NxFr::StringView Path, NxFr::GUID WorldId = WorldId);
		NX_ENGINE_API void SaveScene(NxFr::GUID SceneId);
		NX_ENGINE_API void SaveScenes();
		NX_ENGINE_API Scene* LoadScene(NxFr::GUID SceneId, NxFr::GUID WorldId = WorldId);
		NX_ENGINE_API Scene* LoadSceneSingle(NxFr::GUID SceneId, NxFr::GUID WorldId = WorldId);
		NX_ENGINE_API void UnloadScene(NxFr::GUID SceneId);
		NX_ENGINE_API NxFr::GUID IsSceneLoaded(NxFr::GUID SceneId);
		NX_ENGINE_API NxFr::Array<NxFr::GUID> GetScenes(NxFr::GUID WorldId = 0);

		NX_ENGINE_API Prefab* CreatePrefab(NxFr::Handle<GameObject> Target, NxFr::StringView Path);
		NX_ENGINE_API void SavePrefab(NxFr::Handle<GameObject> Target);
		NX_ENGINE_API Prefab* LoadPrefab(NxFr::GUID PrefabId);
		NX_ENGINE_API void UnloadPrefab(NxFr::GUID PrefabId);
		NX_ENGINE_API void UnpackPrefab(NxFr::Handle<GameObject> Target);
		NX_ENGINE_API NxFr::Handle<GameObject> InstantiatePrefab(Prefab* Instance, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = WorldId);

		NX_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnWorldEvent() { return OnWorldEvent; }
		NX_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnSceneEvent() { return OnSceneEvent; }
		NX_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnGameObjectEvent() { return OnGameObjectEvent; }

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnWorldEvent;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnSceneEvent;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnGameObjectEvent;

		NxFr::Dictionary<NxFr::GUID, World*> Worlds;
		NxFr::Dictionary<NxFr::GUID, NxFr::GUID> Scenes;
	};
}
