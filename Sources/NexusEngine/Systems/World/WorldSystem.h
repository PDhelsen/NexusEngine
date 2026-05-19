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
	class NX_ENGINE_API WorldSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(WorldSystem)

		inline static const NxFr::StringId DummyId = "Dummy"_Sid;
		inline static const NxFr::StringId WorldId = "World"_Sid;

		inline static const NxFr::StringId AppendedId = "Added"_Sid;
		inline static const NxFr::StringId RemovedId = "Removed"_Sid;

		WorldSystem();
		~WorldSystem();

		World* CreateWorld(NxFr::StringId Name, bool References = false);
		World* GetWorld(NxFr::GUID WorldId = WorldId);
		NxFr::Array<NxFr::GUID> GetWorlds();
		void DestroyWorld(NxFr::GUID WorldId);

		Scene* CreateScene(NxFr::StringView Path, NxFr::GUID WorldId = WorldId);
		void SaveScene(NxFr::GUID SceneId);
		void SaveScenes();
		Scene* LoadScene(NxFr::GUID SceneId, NxFr::GUID WorldId = WorldId);
		Scene* LoadSceneSingle(NxFr::GUID SceneId, NxFr::GUID WorldId = WorldId);
		void UnloadScene(NxFr::GUID SceneId);
		NxFr::GUID IsSceneLoaded(NxFr::GUID SceneId);
		NxFr::Array<NxFr::GUID> GetScenes(NxFr::GUID WorldId = 0);

		Prefab* CreatePrefab(NxFr::Handle<GameObject> Target, NxFr::StringView Path);
		void SavePrefab(NxFr::Handle<GameObject> Target);
		Prefab* LoadPrefab(NxFr::GUID PrefabId);
		void UnloadPrefab(NxFr::GUID PrefabId);
		void UnpackPrefab(NxFr::Handle<GameObject> Target);
		NxFr::Handle<GameObject> InstantiatePrefab(Prefab* Instance, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID WorldId = WorldId);

		NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnWorldEvent() { return OnWorldEvent; }
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnSceneEvent() { return OnSceneEvent; }
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID>& GetOnGameObjectEvent() { return OnGameObjectEvent; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnWorldEvent;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnSceneEvent;
		NxFr::Event<NxFr::StringId, NxFr::GUID, NxFr::GUID> OnGameObjectEvent;

		NxFr::Dictionary<NxFr::GUID, World*> Worlds;
		NxFr::Dictionary<NxFr::GUID, NxFr::GUID> Scenes;
	};
}
