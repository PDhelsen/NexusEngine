#pragma once

#include "NexusEngine/Systems/World/World/GameObject.h"

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

		bool Belong(NxFr::Handle<GameObject> Instance) const;
		NxFr::Array<NxFr::Handle<GameObject>> Find(NxFr::StringView Filter) const;

		NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId) const;
		NxFr::Array<NxFr::Handle<GameObject>> GetGameObjects() const;

		NxFr::GUID GetId() const { return WorldId; }

	private:
		NxFr::Handle<GameObject> Allocate(NxFr::GUID GameObjectId);
		void Free(NxFr::Handle<GameObject> Instance);
		void Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index);
		void Detach(NxFr::Handle<GameObject> Instance);

	private:
		NxFr::GUID WorldId;
		bool KeepReferences;
		HandleManager Handles;

		NxFr::List<GameObject> GameObjects;
		NxFr::Dictionary<NxFr::GUID, GameObjectInfo> GameObjectInfos;
		NxFr::Stack<uint64> GameObjectAvailables;
	};
}
