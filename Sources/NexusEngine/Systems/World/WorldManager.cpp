#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldManager.h"

namespace NxEn
{
	WorldManager::WorldManager()
		: Storages(), Objects(), Handles(), WorldInstance(nullptr)
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
		WorldInstance->Tick(TimeStep);
	}

	void WorldManager::Reserve(NxFr::StringId Type, uint64 Size)
	{
		WorldStorage* Storage = GetStorage(Type);
		ResizeStorage(Storage, Size);
	}

	World* WorldManager::CreateWorld(NxFr::StringId Name)
	{
		WorldInstance = new World();
		WorldInstance->WorldId = Name;
		return WorldInstance;
	}

	void WorldManager::DestroyWorld()
	{
		delete WorldInstance;
	}

	World* WorldManager::GetWorld()
	{
		return WorldInstance;
	}

	void WorldManager::SetWorldRoot(NxFr::Handle<GameObject> Instance)
	{
		WorldInstance->Root = Instance;
		if (Instance)
		{
			Instance->SetTickable(true);
		}
	}

	NxFr::Handle<GameObject> WorldManager::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID GameObjectId)
	{
		if (GameObjectId == 0)
		{
			GameObjectId = NxFr::Integer::GenerateGuid();
		}

		WorldStorage* Storage = GetStorage(GameObject::GetClassType());
		NxFr::Handle<GameObject> Instance = AllocateStorage(Storage, GameObjectId);
		Instance->WorldId = WorldInstance->GetId();
		Instance->GameObjectId = GameObjectId;
		Instance->Name = Name;

		if (Parent)
		{
			AttachGameObject(Instance, Parent, Parent->GetChildCount());
		}

		return Instance;
	}

	NxFr::Handle<GameObject> WorldManager::DuplicateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent)
	{
		NxFr::Handle<GameObject> Instance = CreateGameObject("", Parent);
		Instance->Clone((const GameObject*)Original.GetRedirectedPointer());

		NxFr::Handle<GameObject> Child = Target->GetChild();
		while (Child)
		{
			DuplicateGameObject(Child, Instance);
			Child = Child->GetNext();
		}

		return Instance;
	}

	void WorldManager::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		NxFr::Handle<GameObject> Child = Instance->GetChild();
		while (Child)
		{
			NxFr::Handle<GameObject> Next = Child->GetNext();
			DestroyGameObject(Child);
			Child = Next;
		}

		WorldStorage* Storage = GetStorage(GameObject::GetClassType());
		DetachGameObject(Instance);
		FreeStorage(Storage, Instance);
	}

	void WorldManager::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index)
	{
		DetachGameObject(Instance);

		Instance->Parent = Parent;

		if (!Parent->Child)
		{
			Parent->Child = Instance;
		}
		else
		{
			NxFr::Handle<GameObject> Target = Parent->Child;
			while (Target->Next && Index > 0)
			{
				Index--;
				Target = Target->Next;
			}

			if (Index > 0)
			{
				Target->Next = Instance;
				Instance->Prev = Target;
			}
			else
			{
				if (Target->Prev)
				{
					Target->Prev->Next = Instance;
					Instance->Prev = Target->Prev;
				}
				Target->Prev = Instance;
				Instance->Next = Target;
			}
		}
	}

	void WorldManager::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (Instance->Parent && Instance->Parent->Child == Instance)
		{
			Instance->Parent->Child = Instance->Next;
		}
		if (Instance->Prev)
		{
			Instance->Prev->Next = Instance->Next;
		}
		if (Instance->Next)
		{
			Instance->Next->Prev = Instance->Prev;
		}

		Instance->Parent = NxFr::Handle<GameObject>();
		Instance->Prev = NxFr::Handle<GameObject>();
		Instance->Next = NxFr::Handle<GameObject>();
	}

	NxFr::Handle<Object> WorldManager::AllocateStorage(WorldStorage* Storage, NxFr::GUID ObjectId)
	{
		ResizeStorage(Storage, Storage->GetCount() + 1);

		Object* Instance = &Storage->Append();
		uint64 Index = Storage->GetCount() - 1;

		NxFr::Handle<Object> Handle = Handles.AcquireHandle(Instance);
		Objects.Append(ObjectId, WorldObject{ .Id = ObjectId, .Index = Index, .Handle = Handle });

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
