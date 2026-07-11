#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Misc/Iterator.h"

#include "NexusEngine/Systems/World/WorldStorage.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	namespace Iterator
	{
		WorldObject::WorldObject(WorldStorage* Storage, uint64 Index)
			: Storage(Storage), Index(Index)
		{
		}

		bool WorldObject::Equals(const WorldObject& Other) const
		{
			return Storage == Other.Storage && Index == Other.Index;
		}

		void WorldObject::Increment()
		{
			++Index;
		}

		void WorldObject::Decrement()
		{
			--Index;
		}

		Object& WorldObject::Get()
		{
			return Storage->Get(Index);
		}

		const Object& WorldObject::Get() const
		{
			return Storage->Get(Index);
		}

		uint64 WorldObject::Id() const
		{
			return Index;
		}

		WorldHierarchy::WorldHierarchy(NxFr::Handle<GameObject> Instance)
			: Instance(Instance)
		{
		}

		bool WorldHierarchy::Equals(const WorldHierarchy& Other) const
		{
			return Instance == Other.Instance;
		}


		void WorldHierarchy::Increment()
		{
			Instance = Instance->GetIterator();
		}

		void WorldHierarchy::Decrement()
		{
			NX_ASSERT(false, Default, "WorldHierarchy doesn't support decrement");
		}

		GameObject& WorldHierarchy::Get()
		{
			return *Instance.GetRedirectedPointer();
		}

		const GameObject& WorldHierarchy::Get() const
		{
			return *Instance.GetRedirectedPointer();
		}

		uint64 WorldHierarchy::Id() const
		{
			return Instance->GetOrderIndex();
		}
	}
}
