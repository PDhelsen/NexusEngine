#pragma once

#include "NexusEngine/Systems/World/World/GameObject.h"
#include "NexusEngine/Systems/World/Behaviour/Behaviour.h"

namespace NxEn
{
	class ObjectFactory;

	struct FactoryContext
	{
		static ObjectFactory& GetFactory();

		FactoryContext(ObjectFactory& Instance);
		~FactoryContext();
	};

	class ObjectFactory
	{
		struct GameObjectInfo
		{
			uint64 Index;
			NxFr::Handle<GameObject> Handle;
		};

	public:
		ObjectFactory(NxFr::GUID WorldId, bool KeepReferences);
		~ObjectFactory();

		void Reserve(uint64 Size);

		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), NxFr::GUID GameObjectId = 0);
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>(), bool HandleReferences = false);
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);
		void DetachGameObject(NxFr::Handle<GameObject> Instance);

		NxFr::Handle<Behaviour> CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId = 0);
		void DestroyBehaviour(NxFr::Handle<Behaviour> Instance, NxFr::Handle<GameObject> Target);

		bool Belong(NxFr::Handle<GameObject> Instance) const;
		bool Belong(NxFr::Handle<Behaviour> Instance) const;
		NxFr::Array<NxFr::Handle<GameObject>> FindGameObjects(NxFr::StringView Filter) const;
		NxFr::Array<NxFr::Handle<Behaviour>> FindBehaviours(NxFr::StringView Filter) const;

		NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;
		NxFr::Handle<Behaviour> GetBehaviour(NxFr::GUID BehaviourId) const;
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehaviours() const;

		NxFr::Dictionary<NxFr::GUID, GameObjectInfo>::I BeginGameObjects() const { return GameObjectInfos.Begin(); }
		NxFr::Dictionary<NxFr::GUID, GameObjectInfo>::I EndGameObjects() const { return GameObjectInfos.End(); }
		NxFr::Dictionary<NxFr::GUID, NxFr::Handle<Behaviour>>::I BeginBehaviour() const { return BehavioursInfos.Begin(); }
		NxFr::Dictionary<NxFr::GUID, NxFr::Handle<Behaviour>>::I EndBehaviour() const { return BehavioursInfos.End(); }

		NxFr::GUID GetId() const { return WorldId; }

	private:
		NxFr::Handle<GameObject> AllocateGameObject(NxFr::GUID GameObjectId);
		void FreeGameObject(NxFr::Handle<GameObject> Instance);
		NxFr::Handle<Behaviour> AllocateBehaviour(NxFr::StringId Type, NxFr::GUID BehaviourId);
		void FreeBehaviour(NxFr::Handle<Behaviour> Instance);
		void Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index);
		void Detach(NxFr::Handle<GameObject> Instance);

	private:
		NxFr::GUID WorldId;
		bool KeepReferences;
		HandleManager Handles;

		NxFr::List<GameObject> GameObjects;
		NxFr::Dictionary<NxFr::GUID, GameObjectInfo> GameObjectInfos;
		NxFr::Stack<uint64> GameObjectAvailables;

		NxFr::Dictionary<NxFr::StringId, NxFr::List<Behaviour*>> Behaviours;
		NxFr::Dictionary<NxFr::GUID, NxFr::Handle<Behaviour>> BehavioursInfos;
		NxFr::Dictionary<NxFr::StringId, NxFr::Stack<Behaviour*>> BehavioursAvailable;
	};
}
