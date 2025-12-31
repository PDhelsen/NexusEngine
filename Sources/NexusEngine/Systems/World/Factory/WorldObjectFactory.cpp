#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

namespace NxEn
{
	WorldObjectFactory::WorldObjectFactory(NxFr::GUID WorldId, bool KeepReferences)
		: WorldId(WorldId), KeepReferences(KeepReferences), Handles(1024),
		GameObjects(), Behaviours(), Components(),
		InfosGameObjects(), InfosBehaviours(), InfosComponents()
	{
		GameObjects = new WorldObjectStorageTyped<GameObject>(&Handles, &InfosGameObjects);
	}

	WorldObjectFactory::~WorldObjectFactory()
	{
		Clear();
	}

	void WorldObjectFactory::Clear()
	{
		for (auto [Type, Instance] : Components)
		{
			delete Instance;
		}

		for (auto [Type, Instance] : Behaviours)
		{
			delete Instance;
		}

		delete GameObjects;
	}

	NxFr::Handle<GameObject> WorldObjectFactory::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID GameObjectId)
	{
		NxFr::Handle<GameObject> Instance = AllocateGameObject(GameObjectId);

		Instance->SetName(Name);
		if (Parent)
		{
			Attach(Instance, Parent, Parent->GetChildCount());
		}

		WorldObjectReferences* Refs = WorldObjectReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Instance->GetId(), Instance->GetId());
		}

		return Instance;
	}

	NxFr::Handle<GameObject> WorldObjectFactory::DuplicateGameObject(NxFr::Handle<GameObject> Original, NxFr::Handle<GameObject> Parent, bool HandleReferences)
	{
		NxFr::Handle<GameObject> Instance = CreateGameObject("", Parent);
		Instance->Clone((const GameObject*)Original.GetRedirectedPointer());

		WorldObjectReferences* Refs = WorldObjectReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Original->GetId(), Instance->GetId());
		}

		for (auto& B : Original->Behaviours)
		{
			DuplicateBehaviour(B, Instance);
		}

		for (auto& C : Original->Components)
		{
			DuplicateComponent(C, Instance);
		}

		NxFr::Handle<GameObject> Child = Original->GetChild();
		while (Child)
		{
			if (Child->GetReferenceId() && HandleReferences)
			{
				if (KeepReferences)
				{
					NxFr::Handle<GameObject> ChildInstance = CreateGameObject("", Instance);
					ChildInstance->Clone((const GameObject*)Child.GetRedirectedPointer());
				}
				else
				{
					NxFr::Handle<GameObject> ChildTarget = Application::GetSystem<AssetsSystem>()->Load<Prefab>(Child->GetReferenceId())->GetRoot();
					DuplicateGameObject(ChildTarget, Instance, true);
				}
			}
			else
			{
				DuplicateGameObject(Child, Instance, HandleReferences);
			}

			Child = Child->GetNext();
		}

		return Instance;
	}

	void WorldObjectFactory::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		NxFr::Handle<GameObject> Child = Instance->GetChild();
		while (Child)
		{
			NxFr::Handle<GameObject> Next = Child->GetNext();
			DestroyGameObject(Child);
			Child = Next;
		}

		while (Instance->Components.GetCount())
		{
			DestroyComponent(Instance->Components[0]);
		}

		while (Instance->Behaviours.GetCount())
		{
			DestroyBehaviour(Instance->Behaviours[0]);
		}

		Detach(Instance);
		FreeGameObject(Instance);
	}

	void WorldObjectFactory::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Target, int64 Index)
	{
		Detach(Instance);
		Attach(Instance, Target, Index);
	}

	void WorldObjectFactory::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		Detach(Instance);
	}

	NxFr::Handle<Behaviour> WorldObjectFactory::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId)
	{
		NxFr::Handle<Behaviour> Instance = AllocateBehaviour(Type, BehaviourId);

		Instance->Target = Target;
		Target->Behaviours.Append(Instance);

		WorldObjectReferences* Refs = WorldObjectReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Instance->GetId(), Instance->GetId());
		}

		return Instance;
	}

	NxFr::Handle<Behaviour> WorldObjectFactory::DuplicateBehaviour(NxFr::Handle<Behaviour> Original, NxFr::Handle<GameObject> Target)
	{
		NxFr::Handle<Behaviour> Instance = CreateBehaviour(Original->GetObjectType(), Target);
		Instance->Clone((const Behaviour*)Original.GetRedirectedPointer());

		WorldObjectReferences* Refs = WorldObjectReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Original->GetId(), Instance->GetId());
		}

		return Instance;
	}

	void WorldObjectFactory::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NxFr::Handle<GameObject> Target = Instance->GetGameObject();
		uint64 Index = Target->Behaviours.Find(Instance).Id();
		Target->Behaviours.RemoveSwap(Index);
		Instance->Target = NxFr::Handle<GameObject>();

		FreeBehaviour(Instance);
	}

	NxFr::Handle<Component> WorldObjectFactory::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID ComponentId)
	{
		NxFr::Handle<Component> Instance = AllocateComponent(Type, ComponentId);

		Instance->Target = Target;
		Target->Components.Append(Instance);

		WorldObjectReferences* Refs = WorldObjectReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Instance->GetId(), Instance->GetId());
		}

		return Instance;
	}

	NxFr::Handle<Component> WorldObjectFactory::DuplicateComponent(NxFr::Handle<Component> Original, NxFr::Handle<GameObject> Target)
	{
		NxFr::Handle<Component> Instance = CreateComponent(Original->GetObjectType(), Target);
		Instance->Clone((const Component*)Original.GetRedirectedPointer());

		WorldObjectReferences* Refs = WorldObjectReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Original->GetId(), Instance->GetId());
		}

		return Instance;
	}

	void WorldObjectFactory::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		NxFr::Handle<GameObject> Target = Instance->GetGameObject();
		uint64 Index = Target->Components.Find(Instance).Id();
		Target->Components.RemoveSwap(Index);
		Instance->Target = NxFr::Handle<GameObject>();

		FreeComponent(Instance);
	}

	bool WorldObjectFactory::Belong(NxFr::Handle<GameObject> Instance) const
	{
		return Instance->WorldId == WorldId;
	}

	bool WorldObjectFactory::Belong(NxFr::Handle<Behaviour> Instance) const
	{
		return Belong(Instance->GetGameObject());
	}

	bool WorldObjectFactory::Belong(NxFr::Handle<Component> Instance) const
	{
		return Belong(Instance->GetGameObject());
	}

	NxFr::Array<NxFr::Handle<GameObject>> WorldObjectFactory::FindGameObjects(NxFr::StringView Filter) const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result;

		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, " ");
		NxFr::Array<NxFr::GUID> Ids = Filters.GetCount();
		bool All = Filter == "*";

		for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
		{
			Ids[Index] = 0;

			if (NxFr::StringUtility::Start(Filters[Index], "id:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(3, Filters[Index].GetCount() - 3);
				Ids[Index] = NxFr::StringUtility::FromString<NxFr::GUID>(Substring);
			}
		}

		for (auto& [Id, Info] : InfosGameObjects)
		{
			bool MatchId = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				MatchId |= Ids[Index] != 0 && Id == Ids[Index];
				MatchString |= NxFr::StringUtility::Contains(Info.Handle->GetName(), Filters[Index]);
			}

			if (All || MatchId || MatchString)
			{
				Result.Append(InfosGameObjects[Id].Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<GameObject>>(Result);
	}

	NxFr::Array<NxFr::Handle<Behaviour>> WorldObjectFactory::FindBehaviours(NxFr::StringView Filter) const
	{
		NxFr::List<NxFr::Handle<Behaviour>> Result;

		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, " ");
		NxFr::Array<NxFr::StringId> Types = Filters.GetCount();
		NxFr::Array<NxFr::GUID> Ids = Filters.GetCount();
		bool All = Filter == "*";
		bool TypeAndString = false;

		for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
		{
			Types[Index] = 0;
			Ids[Index] = 0;

			if (NxFr::StringUtility::Start(Filters[Index], "t:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(2, Filters[Index].GetCount() - 2);
				Types[Index] = Substring;
				TypeAndString = true;
			}
			else if (NxFr::StringUtility::Start(Filters[Index], "id:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(3, Filters[Index].GetCount() - 3);
				Ids[Index] = NxFr::StringUtility::FromString<NxFr::GUID>(Substring);
			}
		}

		// Check if should filter by type and string;
		TypeAndString &= Filters.GetCount() > 1;

		for (auto [Id, Info] : InfosBehaviours)
		{
			bool MatchId = false;
			bool MatchType = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				MatchId |= Ids[Index] != 0 && Id == Ids[Index];
				MatchType |= Types[Index].GetId() != 0 && Info.Handle->GetObjectType() == Types[Index];
				MatchString |= NxFr::StringUtility::Contains(Info.Handle->GetName(), Filters[Index]);
			}

			if (All || MatchId || (MatchType && !TypeAndString) || (MatchString && !TypeAndString) || (MatchType && MatchString && TypeAndString))
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	NxFr::Array<NxFr::Handle<Component>> WorldObjectFactory::FindComponents(NxFr::StringView Filter) const
	{
		NxFr::List<NxFr::Handle<Component>> Result;

		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, " ");
		NxFr::Array<NxFr::StringId> Types = Filters.GetCount();
		NxFr::Array<NxFr::GUID> Ids = Filters.GetCount();
		bool All = Filter == "*";
		bool TypeAndString = false;

		for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
		{
			Types[Index] = 0;
			Ids[Index] = 0;

			if (NxFr::StringUtility::Start(Filters[Index], "t:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(2, Filters[Index].GetCount() - 2);
				Types[Index] = Substring;
				TypeAndString = true;
			}
			else if (NxFr::StringUtility::Start(Filters[Index], "id:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(3, Filters[Index].GetCount() - 3);
				Ids[Index] = NxFr::StringUtility::FromString<NxFr::GUID>(Substring);
			}
		}

		// Check if should filter by type and string;
		TypeAndString &= Filters.GetCount() > 1;

		for (auto [Id, Info] : InfosComponents)
		{
			bool MatchId = false;
			bool MatchType = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				MatchId |= Ids[Index] != 0 && Id == Ids[Index];
				MatchType |= Types[Index].GetId() != 0 && Info.Handle->GetObjectType() == Types[Index];
				MatchString |= NxFr::StringUtility::Contains(Info.Handle->GetName(), Filters[Index]);
			}

			if (All || MatchId || (MatchType && !TypeAndString) || (MatchString && !TypeAndString) || (MatchType && MatchString && TypeAndString))
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Component>>(Result);
	}

	NxFr::Handle<GameObject> WorldObjectFactory::GetGameObject(NxFr::GUID GameObjectId) const
	{
		const WorldObjectInfo* Info = InfosGameObjects.TryGet(GameObjectId);
		if (Info)
		{
			return Info->Handle;
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<GameObject>> WorldObjectFactory::GetGameObjects() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(InfosGameObjects.GetCount());

		for (auto& [Id, Info] : InfosGameObjects)
		{
			Result.Append(Info.Handle);
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<GameObject>>(Result);
	}

	NxFr::Handle<Behaviour> WorldObjectFactory::GetBehaviour(NxFr::GUID BehaviourId) const
	{
		const WorldObjectInfo* Info = InfosBehaviours.TryGet(BehaviourId);
		if (Info)
		{
			return Info->Handle;
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<Behaviour>> WorldObjectFactory::GetBehaviours() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(InfosBehaviours.GetCount());

		for (auto& [Id, Info] : InfosBehaviours)
		{
			Result.Append(Info.Handle);
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	NxFr::Handle<Component> WorldObjectFactory::GetComponent(NxFr::GUID ComponentId) const
	{
		const WorldObjectInfo* Info = InfosComponents.TryGet(ComponentId);
		if (Info)
		{
			return Info->Handle;
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<Component>> WorldObjectFactory::GetComponents() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(InfosComponents.GetCount());

		for (auto& [Id, Info] : InfosComponents)
		{
			Result.Append(Info.Handle);
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Component>>(Result);
	}

	NxFr::Handle<GameObject> WorldObjectFactory::AllocateGameObject(NxFr::GUID GameObjectId)
	{
		if (GameObjectId == 0)
		{
			GameObjectId = NxFr::Integer::GenerateGuid();
		}

		NxFr::Handle<GameObject> Instance = GameObjects->Allocate(GameObjectId);

		Instance->WorldId = WorldId;
		Instance->GameObjectId = GameObjectId;
		Instance->ReferenceId = 0;

		Instance->Parent = NxFr::Handle<GameObject>();
		Instance->Prev = NxFr::Handle<GameObject>();
		Instance->Next = NxFr::Handle<GameObject>();
		Instance->Child = NxFr::Handle<GameObject>();

		return Instance;
	}

	void WorldObjectFactory::FreeGameObject(NxFr::Handle<GameObject> Instance)
	{
		GameObjects->Free(Instance);
	}

	NxFr::Handle<Behaviour> WorldObjectFactory::AllocateBehaviour(NxFr::StringId Type, NxFr::GUID BehaviourId)
	{
		if (BehaviourId == 0)
		{
			BehaviourId = NxFr::Integer::GenerateGuid();
		}

		WorldObjectStorage* Storage = GetStorage(Type, Behaviours, InfosBehaviours);
		NxFr::Handle<Behaviour> Instance = Storage->Allocate(BehaviourId);

		Instance->BehaviourId = BehaviourId;

		return Instance;
	}

	void WorldObjectFactory::FreeBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		Behaviours[Instance->GetObjectType()]->Free(Instance);
	}

	NxFr::Handle<Component> WorldObjectFactory::AllocateComponent(NxFr::StringId Type, NxFr::GUID ComponentId)
	{
		if (ComponentId == 0)
		{
			ComponentId = NxFr::Integer::GenerateGuid();
		}

		WorldObjectStorage* Storage = GetStorage(Type, Components, InfosComponents);
		NxFr::Handle<Component> Instance = Storage->Allocate(ComponentId);

		Instance->ComponentId = ComponentId;

		return Instance;
	}

	void WorldObjectFactory::FreeComponent(NxFr::Handle<Component> Instance)
	{
		Components[Instance->GetObjectType()]->Free(Instance);
	}

	void WorldObjectFactory::Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index)
	{
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

	void WorldObjectFactory::Detach(NxFr::Handle<GameObject> Instance)
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

	WorldObjectStorage* WorldObjectFactory::GetStorage(NxFr::StringId Type, NxFr::Dictionary<NxFr::StringId, WorldObjectStorage*>& Storages, NxFr::Dictionary<NxFr::GUID, WorldObjectInfo>& Infos)
	{
		WorldObjectStorage** Storage = Storages.TryGet(Type);

		if (!Storage)
		{
			Storage = &Storages.Append(Type, WorldObjectStorage::Create(Type, &Handles, &Infos));
		}

		return *Storage;
	}
}
