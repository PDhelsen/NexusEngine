#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/World/WorldInfo.h"
#include "NexusEngine/Systems/World/WorldStorage.h"
#include "NexusEngine/Systems/World/World.h"
#include "NexusEngine/Systems/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviour.h"
#include "NexusEngine/Systems/World/Component.h"

namespace NxEn
{
	class WorldManager
	{
	public:
		WorldManager();
		~WorldManager();

		void Tick(float TimeStep);
		void Reserve(NxFr::StringId Type, uint64 Size);

		void RecordIds(NxFr::GUID OrignalId, NxFr::GUID InstanceId);
		NxFr::GUID ResolveId(NxFr::GUID InstanceId);
		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>>& GetIdsRemap();

		World* CreateWorld(NxFr::StringId Name);
		void DestroyWorld();
		World* GetWorld();
		void SetWorldRoot(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID GameObjectId = 0);
		NxFr::Handle<GameObject> CreateGameObject(YAML::Node Node, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<const GameObject> Original, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		void DetachGameObject(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId = 0);
		NxFr::Handle<Behaviour> CreateBehaviour(YAML::Node Node, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Behaviour> DuplicateBehaviour(NxFr::Handle<const Behaviour> Original, NxFr::Handle<GameObject> Target);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID ComponentId = 0);
		NxFr::Handle<Component> CreateComponent(YAML::Node Node, NxFr::Handle<GameObject> Target);
		NxFr::Handle<Component> DuplicateComponent(NxFr::Handle<const Component> Original, NxFr::Handle<GameObject> Target);
		void DestroyComponent(NxFr::Handle<Component> Instance);

		bool Belong(NxFr::Handle<Object> Instance);
		WorldObjectType GetType(NxFr::Handle<Object> Instance);
		NxFr::Handle<Object> GetObject(NxFr::GUID ObjectId);
		NxFr::Array<NxFr::Handle<Object>> GetObjects(WorldObjectType Type = WorldObjectType::GameObject);
		NxFr::Array<NxFr::GUID> Find(NxFr::StringView Query, WorldObjectType Type = WorldObjectType::GameObject);
		NxFr::Array<NxFr::GUID> FindGameObjects(NxFr::StringView Query);

		Iterator::WorldObject Begin(NxFr::StringId Type);
		Iterator::WorldObject End(NxFr::StringId Type);

	private:
		NxFr::Handle<Object> Allocate(WorldStorage* Storage, NxFr::GUID ObjectId, WorldObjectType Type);
		void Free(WorldStorage* Storage, NxFr::Handle<Object> Instance);
		void Update(WorldStorage* Storage, uint64 Index);
		void Resize(WorldStorage* Storage, uint64 Size);
		WorldStorage* GetStorage(NxFr::StringId Type);

		NxFr::Dictionary<NxFr::StringId, WorldStorage*> Storages;
		NxFr::Dictionary<NxFr::GUID, WorldObject> Objects;
		NxFr::Context<NxFr::Dictionary<NxFr::GUID, NxFr::GUID>> Remap;
		HandleManager Handles;
		World* WorldInstance;
	};
}

