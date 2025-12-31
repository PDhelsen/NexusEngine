#pragma once

#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

#include "NexusEngine/Systems/World/Factory/WorldObjectInfo.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectStorage.h"

namespace NxEn
{
	class WorldObjectFactory
	{
	public:
		WorldObjectFactory(NxFr::GUID WorldId, bool KeepReferences);
		~WorldObjectFactory();

		void Clear();

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID GameObjectId = 0);
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool HandleReferences = false);
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		void DetachGameObject(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId = 0);
		NxFr::Handle<Behaviour> DuplicateBehaviour(NxFr::Handle<Behaviour> Original, NxFr::Handle<GameObject> Target);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID ComponentId = 0);
		NxFr::Handle<Component> DuplicateComponent(NxFr::Handle<Component> Original, NxFr::Handle<GameObject> Target);
		void DestroyComponent(NxFr::Handle<Component> Instance);

		bool Belong(NxFr::Handle<GameObject> Instance) const;
		bool Belong(NxFr::Handle<Behaviour> Instance) const;
		bool Belong(NxFr::Handle<Component> Instance) const;
		NxFr::Array<NxFr::Handle<GameObject>> FindGameObjects(NxFr::StringView Filter) const;
		NxFr::Array<NxFr::Handle<Behaviour>> FindBehaviours(NxFr::StringView Filter) const;
		NxFr::Array<NxFr::Handle<Component>> FindComponents(NxFr::StringView Filter) const;

		NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;
		NxFr::Handle<Behaviour> GetBehaviour(NxFr::GUID BehaviourId) const;
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehaviours() const;
		NxFr::Handle<Component> GetComponent(NxFr::GUID ComponentId) const;
		NxFr::Array<NxFr::Handle<Component>> GetComponents() const;

		NxFr::GUID GetId() const { return WorldId; }

	private:
		NxFr::Handle<GameObject> AllocateGameObject(NxFr::GUID GameObjectId);
		void FreeGameObject(NxFr::Handle<GameObject> Instance);
		NxFr::Handle<Behaviour> AllocateBehaviour(NxFr::StringId Type, NxFr::GUID BehaviourId);
		void FreeBehaviour(NxFr::Handle<Behaviour> Instance);
		NxFr::Handle<Component> AllocateComponent(NxFr::StringId Type, NxFr::GUID ComponentId);
		void FreeComponent(NxFr::Handle<Component> Instance);
		void Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index);
		void Detach(NxFr::Handle<GameObject> Instance);

	private:
		NxFr::GUID WorldId;
		bool KeepReferences;
		HandleManager Handles;

		WorldObjectStorage* GameObjects;
		NxFr::Dictionary<NxFr::StringId, WorldObjectStorage*> Behaviours;
		NxFr::Dictionary<NxFr::StringId, WorldObjectStorage*> Components;

		NxFr::Dictionary<NxFr::GUID, WorldObjectInfo> InfosGameObjects;
		NxFr::Dictionary<NxFr::GUID, WorldObjectInfo> InfosBehaviours;
		NxFr::Dictionary<NxFr::GUID, WorldObjectInfo> InfosComponents;
	};
}
