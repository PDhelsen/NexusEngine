#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldManager.h"

namespace NxEn
{
	WorldManager::WorldManager()
		: Storages(), Objects(), Handles()
	{
	}

	WorldManager::~WorldManager()
	{
		for (auto [Type, Storage] : Storages)
		{
			delete Storage;
		}
	}

	void WorldManager::Tick(float TimeStep)
	{
	}

	void WorldManager::Reserve(NxFr::StringId Type, uint64 Size)
	{
		WorldStorage* Storage = GetStorage(Type);
		ResizeStorage(Storage, Size);
	}

	NxFr::Handle<Object> WorldManager::AllocateStorage(WorldStorage* Storage, NxFr::GUID Id)
	{
		ResizeStorage(Storage, Storage->GetCount() + 1);

		Object* Instance = &Storage->Append();
		uint64 Index = Storage->GetCount() - 1;

		NxFr::Handle<Object> Handle = Handles.AcquireHandle(Instance);
		Objects.Append(Id, WorldObject{ .Id = Id, .Index = Index, .Handle = Handle });

		return Handle;
	}

	void WorldManager::FreeStorage(WorldStorage* Storage, NxFr::Handle<Object> Instance)
	{
		NxFr::GUID Id = Instance->GetId();
		WorldObject Info = Objects.Get(Id);
		uint64 Index = Info.Index;

		Handles.ReleaseHandle(Instance);
		Objects.Remove(Id);
		Storage->Remove(Info.Index);

		// Remove use RemoveSwap
		// So after calling remove, the last element is now at the removed index
		// So the handle needs to be updated
		UpdateStorage(Storage, Info.Index);
	}

	void WorldManager::UpdateStorage(WorldStorage* Storage, uint64 Index)
	{
		uint64 Count = Storage->GetCount();
		if (Count == 0 || Count == Index)
		{
			return;
		}

		Object& Instance = Storage->Get(Index);
		NxFr::GUID Id = Instance.GetId();

		WorldObject& Info = Objects.Get(Id);
		Info.Index = Index;

		Handles.UpdateHandle(Info.Handle, &Instance);
	}

	void WorldManager::ResizeStorage(WorldStorage* Storage, uint64 Size)
	{
		if (Size < Storage->GetCapacity())
		{
			return;
		}

		Size = NxFr::Math::Max(Size, Storage->GetCapacity() * 2);
		Storage->Reserve(Size);

		for (uint64 Index = 0; Index < Storage->GetCount(); ++Index)
		{
			UpdateStorage(Storage, Index);
		}
	}

	WorldStorage* WorldManager::GetStorage(NxFr::StringId Type)
	{
		WorldStorage** Storage = Storages.TryGet(Type);

		if (!Storage)
		{
			Storage = &Storages.Append(Type, WorldStorage::GetFactory().Create(Type));
		}

		return *Storage;
	}
}
