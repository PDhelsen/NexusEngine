#pragma once

#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class WorldObjectFactory
	{
		struct ObjectInfo
		{
			NxFr::Handle<Object> Handle;
			uint64 Index;
		};

	public:
		WorldObjectFactory(NxFr::GUID WorldId, bool KeepReferences);
		~WorldObjectFactory();

		void Clear();

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID GameObjectId = 0);
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool HandleReferences = false);
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		void DetachGameObject(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId = 0);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance);

		NxFr::Handle<Component> CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID ComponentId = 0);
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

		NxFr::Dictionary<NxFr::GUID, ObjectInfo>::I BeginGameObjects() { return GameObjectInfos.Begin(); }
		NxFr::Dictionary<NxFr::GUID, ObjectInfo>::I EndGameObjects() { return GameObjectInfos.End(); }
		NxFr::Dictionary<NxFr::GUID, ObjectInfo>::I BeginBehaviour() { return BehavioursInfos.Begin(); }
		NxFr::Dictionary<NxFr::GUID, ObjectInfo>::I EndBehaviour() { return BehavioursInfos.End(); }
		template<typename T> NxFr::List<T>::I BeginComponent() { return GetComponentStorage<T>().Begin(); }
		template<typename T> NxFr::List<T>::I EndComponent() { return GetComponentStorage<T>().End(); }

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
		void ReallocateAndUpdateGameObject();
		void ReallocateAndUpdateComponent(NxFr::StringId Type);
		void UpdateComponent(NxFr::StringId Type, uint64 Index);

		template<typename T> ComponentsStorage<T>& GetComponentStorage();

	private:
		NxFr::GUID WorldId;
		bool KeepReferences;
		HandleManager Handles;

		NxFr::List<GameObject> GameObjects;
		NxFr::Dictionary<NxFr::GUID, ObjectInfo> GameObjectInfos;
		NxFr::Stack<uint64> GameObjectAvailables;

		NxFr::Dictionary<NxFr::StringId, NxFr::List<Behaviour*>> Behaviours;
		NxFr::Dictionary<NxFr::GUID, ObjectInfo> BehavioursInfos;
		NxFr::Dictionary<NxFr::StringId, NxFr::Stack<uint64>> BehavioursAvailable;

		NxFr::Dictionary<NxFr::StringId, ComponentsFactory*> Components;
		NxFr::Dictionary<NxFr::GUID, ObjectInfo> ComponentsInfos;
	};

	template<typename T>
	inline ComponentsStorage<T>& WorldObjectFactory::GetComponentStorage()
	{
		NxFr::StringId Type = T::GetClassType();
		ComponentsFactory** Instance = Components.TryGet(Type);

		if (!Instance)
		{
			Instance = &Components.Append(Type, ComponentsFactory::Create(Type));
		}

		return *static_cast<ComponentsStorage<T>*>(*Instance);
	}
}
