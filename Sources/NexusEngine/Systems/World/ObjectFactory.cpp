#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	static NxFr::Stack<ObjectFactory*> Factories;

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
		: WorldId(WorldId), KeepReferences(KeepReferences), Handles(1024), GameObjects(), GameObjectInfos(), GameObjectAvailables()
	{
	}

	ObjectFactory::~ObjectFactory()
	{
		GameObjects.Clear();
	}

	void ObjectFactory::Reserve(uint64 Size)
	{
		Size += GameObjects.GetCount();

		if (Size > GameObjects.GetCapacity())
		{
			Size = NxFr::Math::Max(GameObjects.GetCapacity() * 2, Size);

			GameObjects.Reserve(Size);
			GameObjectInfos.Reserve(Size);

			for (auto& [Id, Info] : GameObjectInfos)
			{
				Handles.UpdateHandle(Info.Handle, &GameObjects[Info.Index]);
			}
		}
	}

	NxFr::Handle<GameObject> ObjectFactory::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent, NxFr::GUID GameObjectId)
	{
		NxFr::Handle<GameObject> Instance = AllocateGameObject(GameObjectId);
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

		Detach(Instance);
		FreeGameObject(Instance);
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
		NxFr::Handle<Behaviour> Instance = AllocateBehaviour(Type, BehaviourId);
		Instance->Target = Target;
		Target->Behaviours.Append(Instance);
		return Instance;
	}

	void ObjectFactory::DestroyBehaviour(NxFr::Handle<Behaviour> Instance, NxFr::Handle<GameObject> Target)
	{
		uint64 Index = Target->Behaviours.Find(Target).Id();
		Target->Behaviours.RemoveSwap(Index);

		Instance->Target = NxFr::Handle<GameObject>();
		FreeBehaviour(Instance);
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

		for (auto& Instance : GameObjects)
		{
			bool MatchId = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				MatchId |= Ids[Index] != 0 && Instance.GetId() == Ids[Index];
				MatchString |= NxFr::StringUtility::Contains(Instance.GetName(), Filters[Index]);
			}

			if (All || MatchId || MatchString)
			{
				Result.Append(GameObjectInfos[Instance.GetId()].Handle);
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

		for (auto [Id, Instance] : Behaviours)
		{
			bool MatchId = false;
			bool MatchType = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				MatchId |= Ids[Index] != 0 && Id == Ids[Index];
				MatchType |= Types[Index].GetId() != 0 && Instance->GetObjectType() == Types[Index];
				MatchString |= NxFr::StringUtility::Contains(Instance->GetGameObject()->GetName(), Filters[Index]);
			}

			if (All || MatchId || (MatchType && !TypeAndString) || (MatchString && !TypeAndString) || (MatchType && MatchString && TypeAndString))
			{
				Result.Append(Instance);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	NxFr::Handle<GameObject> ObjectFactory::GetGameObject(NxFr::GUID GameObjectId) const
	{
		const GameObjectInfo* Info = GameObjectInfos.TryGet(GameObjectId);
		return Info ? Info->Handle : NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::Handle<GameObject>> ObjectFactory::GetGameObjects() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(GameObjects.GetCount());

		for (auto& Instance : GameObjects)
		{
			if (Instance.IsInitialized())
			{
				Result.Append(GameObjectInfos[Instance.GetId()].Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<GameObject>>(Result);
	}

	NxFr::Handle<Behaviour> ObjectFactory::GetBehaviour(NxFr::GUID BehaviourId) const
	{
		const NxFr::Handle<Behaviour>* Instance = Behaviours.TryGet(BehaviourId);
		return Instance ? *Instance : NxFr::Handle<Behaviour>();
	}

	NxFr::Array<NxFr::Handle<Behaviour>> ObjectFactory::GetBehaviours() const
	{
		NxFr::List<NxFr::Handle<GameObject>> Result(Behaviours.GetCount());

		for (auto& [Id, Instance] : Behaviours)
		{
			Result.Append(Instance);
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Behaviour>>(Result);
	}

	NxFr::Handle<GameObject> ObjectFactory::AllocateGameObject(NxFr::GUID GameObjectId)
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
		GameObjectInfos.Append(GameObjectId, GameObjectInfo{ .Index = Index , .Handle = Handle });
		Instance->GameObjectId = GameObjectId;
		Instance->ReferenceId = 0;

		return Handle;
	}

	void ObjectFactory::FreeGameObject(NxFr::Handle<GameObject> Instance)
	{
		NxFr::GUID GameObjectId = Instance->GetId();
		Handles.ReleaseHandle(Instance);

		GameObjectInfo Info = GameObjectInfos[GameObjectId];
		GameObjectInfos.Remove(GameObjectId);

		GameObjectAvailables.Append(Info.Index);
	}

	NxFr::Handle<Behaviour> ObjectFactory::AllocateBehaviour(NxFr::StringId Type, NxFr::GUID BehaviourId)
	{
		if (BehaviourId == 0)
		{
			BehaviourId = NxFr::Integer::GenerateGuid();
		}

		Behaviour* Instance = BehavioursFactory::Create(Type);
		Instance->BehaviourId = BehaviourId;

		NxFr::Handle<Behaviour> Handle = Handles.AcquireHandle(Instance);
		Behaviours.Append(Instance->GetId(), Handle);
		return Handle;
	}

	void ObjectFactory::FreeBehaviour(NxFr::Handle<Behaviour> Target)
	{
		Behaviour* Instance = Target.GetRedirectedPointer();

		Behaviours.Remove(Target->GetId());
		Handles.ReleaseHandle(Target);

		delete Instance;
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
}
