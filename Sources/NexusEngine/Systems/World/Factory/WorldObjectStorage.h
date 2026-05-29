#pragma once

#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectInfo.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectIterator.h"

namespace NxEn
{
	template<typename T>
	class WorldObjectStorageTyped;

	class WorldObjectStorage
	{
	public:
		template<typename T>
		static WorldObjectStorage* Register()
		{
			WorldObjectStorage* Instance = new WorldObjectStorageTyped<T>();
			SetTemplate(T::GetClassType(), Instance);
			return Instance;
		}

		static WorldObjectStorage* GetTemplate(NxFr::StringId Id);
		static void SetTemplate(NxFr::StringId Id, WorldObjectStorage* Instance);
		static WorldObjectStorage* Create(NxFr::StringId Type, HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos);

		WorldObjectStorage(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos);
		virtual ~WorldObjectStorage();

		NxFr::Handle<Object> Allocate(NxFr::GUID Id);
		void Free(NxFr::Handle<Object> Instance);

	protected:
		virtual Object& Append() = 0;
		virtual void Remove(uint64 Index) = 0;
		virtual Object& Get(uint64 Index) = 0;

		virtual void Reserve(uint64 Size) = 0;
		virtual void Clear() = 0;

		virtual bool IsEmpty() const = 0;
		virtual uint64 GetCount() const = 0;
		virtual uint64 GetCapacity() const = 0;

		virtual WorldObjectStorage* Clone(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos) const = 0;

	private:
		void EnsureSlot();
		void UpdateSlot(uint64 Index);

	private:
		HandleManager* Handles;
		NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos;
	};

	template<typename T>
	class WorldObjectStorageTyped : public WorldObjectStorage
	{
	public:
		WorldObjectStorageTyped()
			: WorldObjectStorage(nullptr, nullptr), Instances() { }
		WorldObjectStorageTyped(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos)
			: WorldObjectStorage(Handles, Infos), Instances() { }
		virtual ~WorldObjectStorageTyped() { }

		Object& Append() override { return Instances.AppendConstruct(); }
		void Remove(uint64 Index) override { Instances.RemoveSwap(Index); }
		Object& Get(uint64 Index) override { return Instances.Get(Index); };

		void Reserve(uint64 Size) override { Instances.Reserve(Size); }
		void Clear() override { Instances.Clear(); }

		WorldObjectIterator<T> Begin() { return !IsEmpty() ? WorldObjectIterator<T>(&Instances[0]) : WorldObjectIterator<T>(nullptr); }
		WorldObjectIterator<T> End() { return !IsEmpty() ? WorldObjectIterator(&Instances[0], GetCount()) : WorldObjectIterator<T>(nullptr); }

		bool IsEmpty() const override { return Instances.IsEmpty(); };
		uint64 GetCount() const override { return Instances.GetCount(); }
		uint64 GetCapacity() const override { return Instances.GetCapacity(); }

		WorldObjectStorage* Clone(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos) const override
			{ return new WorldObjectStorageTyped<T>(Handles, Infos); };

	private:
		NxFr::List<T> Instances;
	};
}
