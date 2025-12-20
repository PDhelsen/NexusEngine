#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	ObjectFactory::ObjectFactory(NxFr::GUID WorldId)
		: WorldId(WorldId), Handles(1024), GameObjects(), GameObjectInfos(), GameObjectAvailables(), GameObjectRoot()
	{
		GameObjectRoot = CreateGameObject("Root");
	}

	ObjectFactory::~ObjectFactory()
	{
		GameObjects.Clear();
	}

	NxFr::Handle<GameObject> ObjectFactory::GetGameObject(NxFr::GUID GameObjectId)
	{
		GameObjectInfo* Info = GameObjectInfos.TryGet(GameObjectId);
		return Info ? Info->Handle : NxFr::Handle<GameObject>();
	}

	NxFr::Handle<GameObject> ObjectFactory::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent)
	{
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		NxFr::Handle<GameObject> Instance = Allocate();
		Instance->Initialize();
		Instance->SetName(Name);

		if (!Parent && GameObjectRoot)
		{
			Parent = GameObjectRoot;
		}
		if (Parent)
		{
			Attach(Instance, Parent, Parent->GetChildCount());
		}

		return Instance;
	}

	NxFr::Handle<GameObject> ObjectFactory::DuplicateGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		if (!Parent)
		{
			Parent = Instance->GetParent();
		}

		NxFr::Handle<GameObject> Copy = CreateGameObject(Instance->GetName(), Parent);
		NxFr::Handle<GameObject> Child = Instance->GetChild();
		while (Child)
		{
			DuplicateGameObject(Child, Copy);
			Child = Child->GetNext();
		}

		return Copy;
	}

	void ObjectFactory::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");

		if (!Instance)
		{
			return;
		}

		Detach(Instance);
		Instance->Shutdown();
		Free(Instance);
	}

	void ObjectFactory::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Target, int64 Index)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");
		NEXUS_ASSERT(Belong(Target), Default, "Target should belong to the same Factory");

		if (!Target)
		{
			Target = GameObjectRoot;
		}
		if (Index < 0)
		{
			Index = Target->GetChildCount();
		}

		Detach(Instance);
		Attach(Instance, Target, Index);
	}

	bool ObjectFactory::Belong(NxFr::Handle<GameObject> Instance)
	{
		return Instance->WorldId == WorldId;
	}

	NxFr::Array<NxFr::Handle<GameObject>> ObjectFactory::Find(NxFr::StringView Filter)
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

	NxFr::Handle<GameObject> ObjectFactory::Allocate()
	{
		GameObject* Instance = nullptr;
		NxFr::GUID GameObjectId = NxFr::Integer::GenerateGuid();
		uint64 Index = 0;

		if (GameObjectAvailables.IsEmpty())
		{
			if (GameObjects.GetCount() >= GameObjects.GetCapacity())
			{
				GameObjects.Reserve(GameObjects.GetCapacity() * 2);
				for (auto& [Id, Info] : GameObjectInfos)
				{
					Handles.UpdateHandle(Info.Handle, &GameObjects[Info.Index]);
				}
			}

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

		return Handle;
	}

	void ObjectFactory::Free(NxFr::Handle<GameObject> Instance)
	{
		NxFr::GUID GameObjectId = Instance->GetId();

		GameObjectInfo Info = GameObjectInfos[GameObjectId];
		GameObjectInfos.Remove(GameObjectId);

		GameObjectAvailables.Append(Info.Index);
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
