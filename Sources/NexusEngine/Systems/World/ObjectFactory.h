#pragma once

#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class ObjectFactory
	{
		struct GameObjectInfo
		{
			uint64 Index;
			NxFr::Handle<GameObject> Handle;
		};

	public:
		ObjectFactory(NxFr::GUID WorldId);
		~ObjectFactory();

		NxFr::Handle<GameObject> GetGameObject(NxFr::GUID GameObjectId);
		NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);

		bool Belong(NxFr::Handle<GameObject> Instance);
		NxFr::Array<NxFr::Handle<GameObject>> Find(NxFr::StringView Filter);

		NxFr::List<GameObject>& GetGameObjects() { return GameObjects; };
		NxFr::Handle<GameObject> GetRootGameObject() { return GameObjectRoot; };
		uint64 GetGameObjectsCount() const { return GameObjects.GetCount(); }

	private:
		NxFr::Handle<GameObject> Allocate();
		void Free(NxFr::Handle<GameObject> Instance);
		void Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index);
		void Detach(NxFr::Handle<GameObject> Instance);

	private:
		NxFr::GUID WorldId;
		HandleManager Handles;

		NxFr::List<GameObject> GameObjects;
		NxFr::Dictionary<NxFr::GUID, GameObjectInfo> GameObjectInfos;
		NxFr::Stack<uint64> GameObjectAvailables;
		NxFr::Handle<GameObject> GameObjectRoot;
	};
}
