#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectInfo.h"

namespace NxEn
{
	template<typename T>
	class WorldObjectStorageTyped;

	class WorldObjectStorage
	{
	public:
		template<typename T>
		NEXUS_ENGINE_API static WorldObjectStorage* Register()
		{
			WorldObjectStorage* Instance = new WorldObjectStorageTyped<T>();
			SetTemplate(T::GetClassType(), Instance);
			return Instance;
		}

		NEXUS_ENGINE_API static WorldObjectStorage* GetTemplate(NxFr::StringId Id);
		NEXUS_ENGINE_API static void SetTemplate(NxFr::StringId Id, WorldObjectStorage* Instance);
		NEXUS_ENGINE_API static WorldObjectStorage* Create(NxFr::StringId Type, HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos);

		NEXUS_ENGINE_API WorldObjectStorage(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos);
		NEXUS_ENGINE_API virtual ~WorldObjectStorage();

		NEXUS_ENGINE_API NxFr::Handle<Object> Allocate(NxFr::GUID Id);
		NEXUS_ENGINE_API void Free(NxFr::Handle<Object> Instance);

	protected:
		NEXUS_ENGINE_API virtual Object& Append() = 0;
		NEXUS_ENGINE_API virtual void Remove(uint64 Index) = 0;
		NEXUS_ENGINE_API virtual Object& Get(uint64 Index) = 0;

		NEXUS_ENGINE_API virtual void Reserve(uint64 Size) = 0;
		NEXUS_ENGINE_API virtual void Clear() = 0;

		NEXUS_ENGINE_API virtual uint64 GetCount() = 0;
		NEXUS_ENGINE_API virtual uint64 GetCapacity() = 0;

		NEXUS_ENGINE_API virtual WorldObjectStorage* Clone(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos) const = 0;

	private:
		NEXUS_ENGINE_API void EnsureSlot();
		NEXUS_ENGINE_API void UpdateSlot(uint64 Index);

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

		virtual void Reserve(uint64 Size) override { Instances.Reserve(Size); }
		virtual void Clear() override { Instances.Clear(); }

		virtual uint64 GetCount() override { return Instances.GetCount(); }
		virtual uint64 GetCapacity() override { return Instances.GetCapacity(); }

		virtual WorldObjectStorage* Clone(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos) const override
			{ return new WorldObjectStorageTyped<T>(Handles, Infos); };

	private:
		NxFr::List<T> Instances;
	};
}
