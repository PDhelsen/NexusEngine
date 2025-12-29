#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	static NxFr::Stack<ObjectFactory*> Factories;
	static NxFr::Stack<FactoryReferences*> References;

	ObjectFactory& FactoryContext::GetFactory()
	{
		return *Factories.Get();
	}

	FactoryContext::FactoryContext(ObjectFactory& Instance)
	{
		Factories.Append(&Instance);
	}

	FactoryContext::~FactoryContext()
	{
		Factories.Remove();
	}

	FactoryReferences* FactoryReferences::GetReferences()
	{
		return References.GetCount() ? References.Get() : nullptr;
	}

	NxFr::GUID FactoryReferences::Resolve(NxFr::GUID Id)
	{
		FactoryReferences* Map = GetReferences();
		return Map && Id != 0 ? Map->Ids[Id] : Id;
	}

	FactoryReferences::FactoryReferences()
	{
		References.Append(this);
	}

	FactoryReferences::~FactoryReferences()
	{
		References.Remove();
	}

	ObjectFactory::ObjectFactory(NxFr::GUID WorldId, bool KeepReferences)
		: WorldId(WorldId), KeepReferences(KeepReferences), Handles(1024),
		GameObjects(), GameObjectInfos(), GameObjectAvailables(),
		Behaviours(), BehavioursInfos(), BehavioursAvailable(), BehavioursStarting(),
		Components(), ComponentsInfos()
	{
	}

	ObjectFactory::~ObjectFactory()
	{
		Clear();
	}

	void ObjectFactory::Clear()
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

	void ObjectFactory::Pending()
	{
		ProcessPendings();
	}

	NxFr::Handle<GameObject> ObjectFactory::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID GameObjectId)
	{
		PendingInfo& Info = AllocateGameObject(GameObjectId);

		NxFr::Handle<GameObject> Instance = Info.Handle;
		Instance->SetName(Name);

		if (Parent)
		{
			Attach(Instance, Parent, Parent->GetChildCount());
		}

		FactoryReferences* Refs = FactoryReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Instance->GetId(), Instance->GetId());
		}

		return Instance;
	}

	NxFr::Handle<GameObject> ObjectFactory::DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent, bool HandleReferences)
	{
		FactoryReferences* Refs = FactoryReferences::GetReferences();

		NxFr::Handle<GameObject> Instance = CreateGameObject("", Parent);
		Instance->Clone((const GameObject*)Target.GetRedirectedPointer());
		if (Refs)
		{
			Refs->Ids.Append(Target->GetId(), Instance->GetId());
		}

		for (auto& TargetBehaviour : Target->Behaviours)
		{
			NxFr::Handle<Behaviour> InstanceBehaviour = CreateBehaviour(TargetBehaviour->GetObjectType(), Instance);
			InstanceBehaviour->Clone((const Behaviour*)TargetBehaviour.GetRedirectedPointer());
			if (Refs)
			{
				Refs->Ids.Append(TargetBehaviour->GetId(), InstanceBehaviour->GetId());
			}
		}

		for (auto& TargetComponent : Target->Components)
		{
			NxFr::Handle<Component> InstanceComponent = CreateComponent(TargetComponent->GetObjectType(), Instance);
			InstanceComponent->Clone((const Component*)TargetComponent.GetRedirectedPointer());
			if (Refs)
			{
				Refs->Ids.Append(TargetComponent->GetId(), InstanceComponent->GetId());
			}
		}

		NxFr::Handle<GameObject> Child = Target->GetChild();
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

	void ObjectFactory::DestroyGameObject(NxFr::Handle<GameObject> Instance)
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

		PendingInfo& Info = FreeGameObject(Instance);
	}

	void ObjectFactory::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Target, int64 Index)
	{
		Detach(Instance);
		Attach(Instance, Target, Index);
	}

	void ObjectFactory::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		Detach(Instance);
	}

	NxFr::Handle<Behaviour> ObjectFactory::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID BehaviourId)
	{
		PendingInfo& Info = AllocateBehaviour(Type, BehaviourId);

		NxFr::Handle<Behaviour> Instance = Info.Handle;
		Instance->Target = Target;
		Target->Behaviours.Append(Instance);

		FactoryReferences* Refs = FactoryReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Instance->GetId(), Instance->GetId());
		}

		return Instance;
	}

	void ObjectFactory::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NxFr::Handle<GameObject> Target = Instance->GetGameObject();
		uint64 Index = Target->Behaviours.Find(Instance).Id();
		Target->Behaviours.RemoveSwap(Index);
		Instance->Target = NxFr::Handle<GameObject>();

		PendingInfo& Info = FreeBehaviour(Instance);
	}

	NxFr::Handle<Component> ObjectFactory::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target, NxFr::GUID ComponentId)
	{
		PendingInfo& Info = AllocateComponent(Type, ComponentId);

		NxFr::Handle<Component> Instance = Info.Handle;
		Instance->Target = Target;
		Target->Components.Append(Instance);

		FactoryReferences* Refs = FactoryReferences::GetReferences();
		if (Refs)
		{
			Refs->Ids.Append(Instance->GetId(), Instance->GetId());
		}

		return Instance;
	}

	void ObjectFactory::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		NxFr::Handle<GameObject> Target = Instance->GetGameObject();
		uint64 Index = Target->Components.Find(Instance).Id();
		Target->Components.RemoveSwap(Index);
		Instance->Target = NxFr::Handle<GameObject>();

		PendingInfo& Info = FreeComponent(Instance);
	}

	bool ObjectFactory::Belong(NxFr::Handle<GameObject> Instance) const
	{
		return Instance->WorldId == WorldId;
	}

	bool ObjectFactory::Belong(NxFr::Handle<Behaviour> Instance) const
	{
		return Belong(Instance->GetGameObject());
	}

	bool ObjectFactory::Belong(NxFr::Handle<Component> Instance) const
	{
		return Belong(Instance->GetGameObject());
	}

	NxFr::Array<NxFr::Handle<GameObject>> ObjectFactory::FindGameObjects(NxFr::StringView Filter) const
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

	NxFr::Array<NxFr::Handle<Behaviour>> ObjectFactory::FindBehaviours(NxFr::StringView Filter) const
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

	NxFr::Array<NxFr::Handle<Component>> ObjectFactory::FindComponents(NxFr::StringView Filter) const
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

	NxFr::Handle<GameObject> ObjectFactory::GetGameObject(NxFr::GUID GameObjectId) const
	{
		const ObjectInfo* OInfo = GameObjectInfos.TryGet(GameObjectId);
		if (OInfo)
		{
			return OInfo->Handle;
		}

		for (auto& PInfo : Pendings)
		{
			if (PInfo.Id == GameObjectId)
			{
				return PInfo.Handle;
			}
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<GameObject>> ObjectFactory::GetGameObjects() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(GameObjectInfos.GetCount());

		for (auto& [Id, Info] : GameObjectInfos)
		{
			Result.Append(Info.Handle);
		}

		for (auto& Info : Pendings)
		{
			if (Info.Type == PendingInfo::InfoType::GameObject)
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<GameObject>>(Result);
	}

	NxFr::Handle<Behaviour> ObjectFactory::GetBehaviour(NxFr::GUID BehaviourId) const
	{
		const ObjectInfo* OInfo = BehavioursInfos.TryGet(BehaviourId);
		if (OInfo)
		{
			return OInfo->Handle;
		}

		for (auto& PInfo : Pendings)
		{
			if (PInfo.Id == BehaviourId)
			{
				return PInfo.Handle;
			}
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<Behaviour>> ObjectFactory::GetBehaviours() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(BehavioursInfos.GetCount());

		for (auto& [Id, Info] : BehavioursInfos)
		{
			Result.Append(Info.Handle);
		}

		for (auto& Info : Pendings)
		{
			if (Info.Type == PendingInfo::InfoType::Behaviour)
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	NxFr::Handle<Component> ObjectFactory::GetComponent(NxFr::GUID ComponentId) const
	{
		const ObjectInfo* OInfo = ComponentsInfos.TryGet(ComponentId);
		if (OInfo)
		{
			return OInfo->Handle;
		}

		for (auto& PInfo : Pendings)
		{
			if (PInfo.Id == ComponentId)
			{
				return PInfo.Handle;
			}
		}

		return NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<Component>> ObjectFactory::GetComponents() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(ComponentsInfos.GetCount());

		for (auto& [Id, Info] : ComponentsInfos)
		{
			Result.Append(Info.Handle);
		}

		for (auto& Info : Pendings)
		{
			if (Info.Type == PendingInfo::InfoType::Component)
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Component>>(Result);
	}

	ObjectFactory::PendingInfo& ObjectFactory::AllocateGameObject(NxFr::GUID GameObjectId)
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
		Instance->WorldId = WorldId;
		Instance->GameObjectId = GameObjectId;
		Instance->ReferenceId = 0;

		Instance->Parent = NxFr::Handle<GameObject>();
		Instance->Prev = NxFr::Handle<GameObject>();
		Instance->Next = NxFr::Handle<GameObject>();
		Instance->Child = NxFr::Handle<GameObject>();

		return Pendings.Append(PendingInfo{
			.Id = GameObjectId,
			.Handle = Handle,
			.Index = Index,
			.Type = PendingInfo::InfoType::GameObject,
			.Start = true
			});
	}

	ObjectFactory::PendingInfo& ObjectFactory::FreeGameObject(NxFr::Handle<GameObject> Instance)
	{
		NxFr::GUID GameObjectId = Instance->GetId();
		ObjectInfo Info = GameObjectInfos[GameObjectId];

		Handles.ReleaseHandle(Instance);
		GameObjectAvailables.Append(Info.Index);

		return Pendings.Append(PendingInfo{
			.Id = GameObjectId,
			.Handle = NxFr::Handle<GameObject>(),
			.Index = Info.Index,
			.Type = PendingInfo::InfoType::GameObject,
			.Start = false
			});
	}

	ObjectFactory::PendingInfo& ObjectFactory::AllocateBehaviour(NxFr::StringId Type, NxFr::GUID BehaviourId)
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
		BehavioursStarting.Append(Handle);
		Instance->BehaviourId = BehaviourId;

		return Pendings.Append(PendingInfo{
			.Id = BehaviourId,
			.Handle = Handle,
			.Index = Index,
			.Type = PendingInfo::InfoType::Behaviour,
			.Start = true
		});
	}

	ObjectFactory::PendingInfo& ObjectFactory::FreeBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NxFr::StringId Type = Instance->GetObjectType();
		NxFr::GUID BehaviourId = Instance->GetId();
		ObjectInfo Info = BehavioursInfos[BehaviourId];
		uint64 StartingIndex = BehavioursStarting.Find(Instance).Id();

		if (StartingIndex != BehavioursStarting.GetCount())
		{
			BehavioursStarting.RemoveSwap(StartingIndex);
		}
		Handles.ReleaseHandle(Instance);

		if (!BehavioursAvailable.ContainsKey(Type))
		{
			BehavioursAvailable.Append(Type, NxFr::Stack<uint64>());
		}
		BehavioursAvailable[Type].Append(Info.Index);

		return Pendings.Append(PendingInfo{
			.Id = BehaviourId,
			.Handle = NxFr::Handle<Behaviour>(),
			.Index = Info.Index,
			.Type = PendingInfo::InfoType::Behaviour,
			.Start = false
		});
	}

	ObjectFactory::PendingInfo& ObjectFactory::AllocateComponent(NxFr::StringId Type, NxFr::GUID ComponentId)
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
		BehavioursStarting.Append(Handle);
		Instance->ComponentId = ComponentId;

		return Pendings.Append(PendingInfo{
			.Id = ComponentId,
			.Handle = Handle,
			.Index = Index,
			.Type = PendingInfo::InfoType::Component,
			.Start = true
			});
	}

	ObjectFactory::PendingInfo& ObjectFactory::FreeComponent(NxFr::Handle<Component> Instance)
	{
		NxFr::StringId Type = Instance->GetObjectType();
		NxFr::GUID ComponentId = Instance->GetId();
		ObjectInfo Info = ComponentsInfos[ComponentId];

		Handles.ReleaseHandle(Instance);

		// Component Remove use a RemoveSwap
		// So after the remove, the last element is now at Info.Index
		// Since that element was moved, its handle has to be updated 
		Components[Type]->Remove(Info.Index);
		UpdateComponent(Type, Info.Index);

		return Pendings.Append(PendingInfo{
			.Id = ComponentId,
			.Handle = NxFr::Handle<Component>(),
			.Index = Info.Index,
			.Type = PendingInfo::InfoType::Component,
			.Start = false
			});
	}

	void ObjectFactory::Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index)
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

	void ObjectFactory::Detach(NxFr::Handle<GameObject> Instance)
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

	void ObjectFactory::ReallocateAndUpdateGameObject()
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

		for (auto& Info : Pendings)
		{
			if (Info.Type == PendingInfo::InfoType::GameObject)
			{
				Handles.UpdateHandle(static_cast<NxFr::Handle<Object>>(Info.Handle), static_cast<Object*>(&GameObjects[Info.Index]));
			}
		}
	}

	void ObjectFactory::ReallocateAndUpdateComponent(NxFr::StringId Type)
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

		for (auto& Info : Pendings)
		{
			if (Info.Type == PendingInfo::InfoType::Component)
			{
				Handles.UpdateHandle(static_cast<NxFr::Handle<Object>>(Info.Handle), static_cast<Object*>(Components[Type]->Get(Info.Index)));
			}
		}
	}

	void ObjectFactory::UpdateComponent(NxFr::StringId Type, uint64 Index)
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

	void ObjectFactory::ProcessPendings()
	{
		for (uint64 Index = Pendings.GetCount(); Index > 0; --Index)
		{
			PendingInfo& Info = Pendings[Index - 1];
			NxFr::Dictionary<NxFr::GUID, ObjectInfo>* Infos = nullptr;

			switch (Info.Type)
			{
			case PendingInfo::InfoType::GameObject: Infos = &GameObjectInfos; break;
			case PendingInfo::InfoType::Behaviour: Infos = &BehavioursInfos;  break;
			case PendingInfo::InfoType::Component: Infos = &ComponentsInfos;  break;
			}

			if (Info.Start)
			{
				Infos->Append(Info.Id, ObjectInfo{ .Handle = Info.Handle, .Index = Info.Index });
			}
			else
			{
				Infos->Remove(Info.Id);
			}

			Pendings.Remove(Index - 1);
		}
	}
}
