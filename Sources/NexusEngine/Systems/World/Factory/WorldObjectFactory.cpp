#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

#include "NexusEngine/Systems/World/Factory/WorldObjectReferences.h"

namespace NxEn
{
	WorldObjectFactory::WorldObjectFactory(NxFr::GUID WorldId, bool KeepReferences)
		: WorldId(WorldId), KeepReferences(KeepReferences), Handles(1024),
		GameObjects(), GameObjectInfos(), GameObjectAvailables(),
		Behaviours(), BehavioursInfos(), BehavioursAvailable(),
		Components(), ComponentsInfos()
	{
	}

	WorldObjectFactory::~WorldObjectFactory()
	{
		Clear();
	}

	void WorldObjectFactory::Clear()
	{
		for (auto [Id, Components] : Components)
		{
			delete Components;
		}

		for (auto& [Id, Instances] : Behaviours)
		{
			for (auto Instance : Instances)
			{
				delete Instance;
			}

			Instances.Clear();
		}

		for (auto& [Id, Instances] : BehavioursAvailable)
		{
			Instances.Clear();
		}

		Components.Clear();
		ComponentsInfos.Clear();

		Behaviours.Clear();
		BehavioursInfos.Clear();
		BehavioursAvailable.Clear();

		GameObjects.Clear();
		GameObjectInfos.Clear();
		GameObjectAvailables.Clear();
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
			DuplicateBehaviour(B, Original);
		}

		for (auto& C : Original->Components)
		{
			DuplicateComponent(C, Original);
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

		for (auto& [Id, Info] : GameObjectInfos)
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
				Result.Append(GameObjectInfos[Id].Handle);
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

		for (auto [Id, Info] : BehavioursInfos)
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

		for (auto [Id, Info] : ComponentsInfos)
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
		const ObjectInfo* Info = GameObjectInfos.TryGet(GameObjectId);
		if (Info)
		{
			return Info->Handle;
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<GameObject>> WorldObjectFactory::GetGameObjects() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(GameObjectInfos.GetCount());

		for (auto& [Id, Info] : GameObjectInfos)
		{
			Result.Append(Info.Handle);
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<GameObject>>(Result);
	}

	NxFr::Handle<Behaviour> WorldObjectFactory::GetBehaviour(NxFr::GUID BehaviourId) const
	{
		const ObjectInfo* Info = BehavioursInfos.TryGet(BehaviourId);
		if (Info)
		{
			return Info->Handle;
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<Behaviour>> WorldObjectFactory::GetBehaviours() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(BehavioursInfos.GetCount());

		for (auto& [Id, Info] : BehavioursInfos)
		{
			Result.Append(Info.Handle);
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	NxFr::Handle<Component> WorldObjectFactory::GetComponent(NxFr::GUID ComponentId) const
	{
		const ObjectInfo* Info = ComponentsInfos.TryGet(ComponentId);
		if (Info)
		{
			return Info->Handle;
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<Component>> WorldObjectFactory::GetComponents() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(ComponentsInfos.GetCount());

		for (auto& [Id, Info] : ComponentsInfos)
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

		GameObject* Instance = nullptr;
		uint64 Index = 0;

		if (GameObjectAvailables.IsEmpty())
		{
			ReallocateAndUpdateGameObject();

			Index = GameObjects.GetCount();
			Instance = &GameObjects.AppendConstruct(WorldId);
		}
		else
		{
			Index = GameObjectAvailables.Get();
			Instance = &GameObjects[Index];

			GameObjectAvailables.Remove();
		}

		NxFr::Handle<GameObject> Handle = Handles.AcquireHandle(Instance);
		GameObjectInfos.Append(GameObjectId, ObjectInfo{ .Handle = Handle, .Index = Index });

		Instance->WorldId = WorldId;
		Instance->GameObjectId = GameObjectId;
		Instance->ReferenceId = 0;

		Instance->Parent = NxFr::Handle<GameObject>();
		Instance->Prev = NxFr::Handle<GameObject>();
		Instance->Next = NxFr::Handle<GameObject>();
		Instance->Child = NxFr::Handle<GameObject>();

		return Handle;
	}

	void WorldObjectFactory::FreeGameObject(NxFr::Handle<GameObject> Instance)
	{
		NxFr::GUID GameObjectId = Instance->GetId();
		ObjectInfo Info = GameObjectInfos[GameObjectId];

		Handles.ReleaseHandle(Instance);
		GameObjectInfos.Remove(GameObjectId);

		GameObjectAvailables.Append(Info.Index);
	}

	NxFr::Handle<Behaviour> WorldObjectFactory::AllocateBehaviour(NxFr::StringId Type, NxFr::GUID BehaviourId)
	{
		if (BehaviourId == 0)
		{
			BehaviourId = NxFr::Integer::GenerateGuid();
		}

		Behaviour* Instance = nullptr;
		uint64 Index = 0;

		if (!BehavioursAvailable.ContainsKey(Type) || BehavioursAvailable[Type].IsEmpty())
		{
			Instance = BehavioursFactory::Create(Type);
			if (!Behaviours.ContainsKey(Type))
			{
				Behaviours.Append(Type, NxFr::List<Behaviour*>());
			}

			Index = Behaviours[Type].GetCount();
			Behaviours[Type].Append(Instance);
		}
		else
		{
			Index = BehavioursAvailable[Type].Get();
			Instance = Behaviours[Type][Index];

			BehavioursAvailable[Type].Remove();
		}

		NxFr::Handle<Behaviour> Handle = Handles.AcquireHandle(Instance);
		BehavioursInfos.Append(BehaviourId, ObjectInfo{ .Handle = Handle,.Index = Index });

		Instance->BehaviourId = BehaviourId;

		return Handle;
	}

	void WorldObjectFactory::FreeBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NxFr::StringId Type = Instance->GetObjectType();
		NxFr::GUID BehaviourId = Instance->GetId();
		ObjectInfo Info = BehavioursInfos[BehaviourId];

		Handles.ReleaseHandle(Instance);
		BehavioursInfos.Remove(BehaviourId);

		if (!BehavioursAvailable.ContainsKey(Type))
		{
			BehavioursAvailable.Append(Type, NxFr::Stack<uint64>());
		}
		BehavioursAvailable[Type].Append(Info.Index);
	}

	NxFr::Handle<Component> WorldObjectFactory::AllocateComponent(NxFr::StringId Type, NxFr::GUID ComponentId)
	{
		if (ComponentId == 0)
		{
			ComponentId = NxFr::Integer::GenerateGuid();
		}

		Component* Instance = nullptr;
		uint64 Index = 0;

		if (!Components.ContainsKey(Type))
		{
			Components.Append(Type, ComponentsFactory::Create(Type));
		}
		else
		{
			ReallocateAndUpdateComponent(Type);
		}

		Instance = Components[Type]->Append();
		Index = Components[Type]->GetCount() - 1;

		NxFr::Handle<Behaviour> Handle = Handles.AcquireHandle(Instance);
		ComponentsInfos.Append(ComponentId, ObjectInfo{ .Handle = Handle,.Index = Index });

		Instance->ComponentId = ComponentId;

		return Handle;
	}

	void WorldObjectFactory::FreeComponent(NxFr::Handle<Component> Instance)
	{
		NxFr::StringId Type = Instance->GetObjectType();
		NxFr::GUID ComponentId = Instance->GetId();
		ObjectInfo Info = ComponentsInfos[ComponentId];

		Handles.ReleaseHandle(Instance);
		ComponentsInfos.Remove(ComponentId);

		// Component Remove use a RemoveSwap
		// So after the remove, the last element is now at Info.Index
		// Since that element was moved, its handle has to be updated 
		Components[Type]->Remove(Info.Index);
		UpdateComponent(Type, Info.Index);
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

	void WorldObjectFactory::ReallocateAndUpdateGameObject()
	{
		uint64 Size = 1 + GameObjects.GetCount();
		if (Size < GameObjects.GetCapacity())
		{
			return;
		}

		Size = NxFr::Math::Max(GameObjects.GetCapacity() * 2, Size);

		GameObjects.Reserve(Size);
		GameObjectInfos.Reserve(Size);

		for (auto& [Id, Info] : GameObjectInfos)
		{
			Handles.UpdateHandle(static_cast<NxFr::Handle<Object>>(Info.Handle), static_cast<Object*>(&GameObjects[Info.Index]));
		}
	}

	void WorldObjectFactory::ReallocateAndUpdateComponent(NxFr::StringId Type)
	{
		uint64 Size = 1 + Components[Type]->GetCount();
		if (Size < Components[Type]->GetCapacity())
		{
			return;
		}

		Size = NxFr::Math::Max(GameObjects.GetCapacity() * 2, Size);

		Components[Type]->Reserve(Size);
		ComponentsInfos.Reserve(Size);

		for (auto& [Id, Info] : ComponentsInfos)
		{
			Handles.UpdateHandle(static_cast<NxFr::Handle<Object>>(Info.Handle), static_cast<Object*>(Components[Type]->Get(Info.Index)));
		}
	}

	void WorldObjectFactory::UpdateComponent(NxFr::StringId Type, uint64 Index)
	{
		uint64 Count = Components[Type]->GetCount();
		if (Count == 0 || Count == Index)
		{
			return;
		}

		NxFr::GUID Id = Components[Type]->Get(Index)->GetId();
		ObjectInfo& Info = ComponentsInfos[Id];

		Info.Index = Index;
		Handles.UpdateHandle(static_cast<NxFr::Handle<Object>>(Info.Handle), static_cast<Object*>(Components[Type]->Get(Info.Index)));
	}
}
