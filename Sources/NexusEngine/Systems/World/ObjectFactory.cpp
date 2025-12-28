#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	static NxFr::Stack<ObjectFactory*> Factories;

	ObjectFactory& FactoryContext::GetAssetsFactory()
	{
		static ObjectFactory AssetsFactory("Assets"_Sid, true);
		return AssetsFactory;
	}

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

	ObjectFactory::ObjectFactory(NxFr::GUID WorldId, bool KeepReferences)
		: WorldId(WorldId), KeepReferences(KeepReferences), Handles(1024),
		GameObjects(), GameObjectInfos(), GameObjectAvailables(),
		Behaviours(), BehavioursInfos(), BehavioursAvailable(), BehavioursStarting()
	{
	}

	ObjectFactory::~ObjectFactory()
	{
		Clear();
	}

	void ObjectFactory::Clear()
	{
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

		Behaviours.Clear();
		BehavioursInfos.Clear();
		BehavioursAvailable.Clear();

		GameObjects.Clear();
		GameObjectInfos.Clear();
		GameObjectAvailables.Clear();
	}

	void ObjectFactory::Reserve(uint64 Size)
	{
		Size += GameObjects.GetCount();
		if (Size > GameObjects.GetCapacity())
		{
			ReallocateAndUpdateHandles(Size);
		}
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

		return Instance;
	}

	NxFr::Handle<GameObject> ObjectFactory::DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent, bool HandleReferences)
	{
		NxFr::Handle<GameObject> Instance = CreateGameObject("", Parent);
		Instance->Clone((const GameObject*)Target.GetRedirectedPointer());

		for (auto& TargetBehaviour : Target->Behaviours)
		{
			NxFr::Handle<Behaviour> InstanceBehaviour = CreateBehaviour(TargetBehaviour->GetObjectType(), Instance);
			InstanceBehaviour->Clone((const Behaviour*)TargetBehaviour.GetRedirectedPointer());
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

		while (Instance->Behaviours.GetCount())
		{
			DestroyBehaviour(Instance->Behaviours[0], Instance);
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
		return Instance;
	}

	void ObjectFactory::DestroyBehaviour(NxFr::Handle<Behaviour> Instance, NxFr::Handle<GameObject> Target)
	{
		uint64 Index = Target->Behaviours.Find(Instance).Id();
		Target->Behaviours.RemoveSwap(Index);
		Instance->Target = NxFr::Handle<GameObject>();

		PendingInfo& Info = FreeBehaviour(Instance);
	}

	bool ObjectFactory::Belong(NxFr::Handle<GameObject> Instance) const
	{
		return Instance->WorldId == WorldId;
	}

	bool ObjectFactory::Belong(NxFr::Handle<Behaviour> Instance) const
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
			if (!Info.IsBehaviour)
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
			if (Info.IsBehaviour)
			{
				Result.Append(Info.Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
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
			Reserve(1);

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
		Instance->GameObjectId = GameObjectId;
		Instance->ReferenceId = 0;

		return Pendings.Append(PendingInfo{
			.Id = GameObjectId,
			.Handle = Handle,
			.Index = Index,
			.Start = true,
			.IsBehaviour = false
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
			.Start = false,
			.IsBehaviour = false
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
			.Start = true,
			.IsBehaviour = true
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
			.Handle = NxFr::Handle<GameObject>(),
			.Index = Info.Index,
			.Start = false,
			.IsBehaviour = true
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

	void ObjectFactory::ReallocateAndUpdateHandles(uint64 Size)
	{
		Size = NxFr::Math::Max(GameObjects.GetCapacity() * 2, Size);

		GameObjects.Reserve(Size);
		GameObjectInfos.Reserve(Size);

		for (auto& [Id, Info] : GameObjectInfos)
		{
			Handles.UpdateHandle(static_cast<NxFr::Handle<GameObject>>(Info.Handle), &GameObjects[Info.Index]);
		}
	}

	void ObjectFactory::ProcessPendings()
	{
		for (uint64 Index = Pendings.GetCount(); Index > 0; --Index)
		{
			PendingInfo& Info = Pendings[Index - 1];
			NxFr::Dictionary<NxFr::GUID, ObjectInfo>& Infos = Info.IsBehaviour ? BehavioursInfos : GameObjectInfos;

			if (Info.Start)
			{
				Infos.Append(Info.Id, ObjectInfo{ .Handle = Info.Handle, .Index = Info.Index });
			}
			else
			{
				Infos.Remove(Info.Id);
			}

			Pendings.Remove(Index - 1);
		}
	}
}
