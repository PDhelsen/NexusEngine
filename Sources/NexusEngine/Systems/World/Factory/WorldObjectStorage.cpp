#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectStorage.h"

namespace NxEn
{
	static NxFr::Dictionary<NxFr::StringId, WorldObjectStorage*>& GetStorageTemplates()
	{
		static NxFr::Dictionary<NxFr::StringId, WorldObjectStorage*> Templates;
		return Templates;
	}

	WorldObjectStorage* WorldObjectStorage::GetTemplate(NxFr::StringId Id)
	{
		return GetStorageTemplates()[Id];
	}

	void WorldObjectStorage::SetTemplate(NxFr::StringId Id, WorldObjectStorage* Instance)
	{
		GetStorageTemplates().AppendOrAssign(Id, Instance);
	}

	WorldObjectStorage* WorldObjectStorage::Create(NxFr::StringId Type, HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos)
	{
		WorldObjectStorage* Template = GetTemplate(Type);
		return Template->Clone(Handles, Infos);
	}

	WorldObjectStorage::WorldObjectStorage(HandleManager* Handles, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>* Infos)
		: Handles(Handles), Infos(Infos)
	{
	}

	WorldObjectStorage::~WorldObjectStorage()
	{
	}

	NxFr::Handle<Object> WorldObjectStorage::Allocate(NxFr::GUID Id)
	{
		EnsureSlot();

		Object* Instance = &Append();
		uint64 Index = GetCount() - 1;

		NxFr::Handle<Behaviour> Handle = Handles->AcquireHandle(Instance);
		Infos->Append(Id, WorldObjectInfo{ .Handle = Handle,.Index = Index });

		return Handle;
	}

	void WorldObjectStorage::Free(NxFr::Handle<Object> Instance)
	{
		NxFr::StringId Type = Instance->GetObjectType();
		NxFr::GUID Id = Instance->GetId();
		WorldObjectInfo Info = Infos->Get(Id);
		uint64 Index = Info.Index;

		Handles->ReleaseHandle(Instance);
		Infos->Remove(Id);
		Remove(Info.Index);

		// Remove use RemoveSwap
		// So after calling remove, the last element is now at the removed index
		// So the handle needs to be updated
		UpdateSlot(Info.Index);
	}

	void WorldObjectStorage::EnsureSlot()
	{
		uint64 Size = GetCount() + 1;
		if (Size < GetCapacity())
		{
			return;
		}

		Size = NxFr::Math::Max(GetCapacity() * 2, Size);

		Reserve(Size);
		Reserve(Size);

		for (uint64 Index = 0; Index < GetCount(); ++Index)
		{
			UpdateSlot(Index);
		}
	}

	void WorldObjectStorage::UpdateSlot(uint64 Index)
	{
		uint64 Count = GetCount();
		if (Count == 0 || Count == Index)
		{
			return;
		}
		
		Object& Instance = Get(Index);
		NxFr::GUID Id = Instance.GetId();
		
		WorldObjectInfo& Info = Infos->Get(Id);
		Info.Index = Index;

		Handles->UpdateHandle(Info.Handle, &Instance);
	}
}
