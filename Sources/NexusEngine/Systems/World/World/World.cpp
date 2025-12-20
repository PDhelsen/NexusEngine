#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(World)

	World::World(NxFr::StringId Id)
		: Id(Id), Name(Id.GetString()), Objects(), Root(), Infos(), Availables(), Handles(1024)
	{
		SetTickable(true);
	}

	World::~World()
	{
	}

	NxFr::Handle<GameObject> World::GetGameObject(NxFr::GUID Id)
	{
		Info* Info = Infos.TryGet(Id);
		return Info ? Info->Handle : NxFr::Handle<GameObject>();
	}

	NxFr::Handle<GameObject> World::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent)
	{
		NxFr::Handle<GameObject> Instance = Allocate();
		Instance->Initialize();
		Instance->SetName(Name);

		if (!Parent && Root)
		{
			Parent = Root;
		}
		if (Parent)
		{
			Attach(Instance, Parent, Parent->GetChildCount());
		}

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, Id, Instance->GetId());
		return Instance;
	}

	NxFr::Handle<GameObject> World::DuplicateGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent)
	{
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

	void World::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance)
		{
			return;
		}

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::RemovedId, Id, Instance->GetId());

		Detach(Instance);
		Instance->Shutdown();
		Free(Instance);
	}

	void World::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Target, int64 Index)
	{
		if (!Target)
		{
			Target = Root;
		}
		if (Index < 0)
		{
			Index = Target->GetChildCount();
		}

		Detach(Instance);
		Attach(Instance, Target, Index);

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::ChangedId, Id, Instance->GetId());
	}

	bool World::Belong(NxFr::Handle<GameObject> Instance)
	{
		return Instance->WorldId == Id;
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::Find(NxFr::StringView Filter)
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

		for (auto& Instance : Objects)
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
				Result.Append(Infos[Instance.GetId()].Handle);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<GameObject>>(Result);
	}

	void World::OnInitialize()
	{
		Root = CreateGameObject(Id.GetString() + " Root");
	}

	void World::OnShutdown()
	{
		Objects.Clear();
	}

	void World::OnTick(float TimeStep)
	{
		for (auto& Instance : Objects)
		{
			Instance.Tick(TimeStep);
		}
	}

	NxFr::Handle<GameObject> World::Allocate()
	{
		GameObject* Instance = nullptr;
		NxFr::GUID GameObjectId = NxFr::Integer::GenerateGuid();
		uint64 Index = 0;

		if (Availables.IsEmpty())
		{
			if (Objects.GetCount() >= Objects.GetCapacity())
			{
				Objects.Reserve(Objects.GetCapacity() * 2);
				for (auto& [Id, Info] : Infos)
				{
					Handles.UpdateHandle(Info.Handle, &Objects[Info.Index]);
				}
			}

			Index = Objects.GetCount();
			Instance = &Objects.AppendConstruct(Id);
		}
		else
		{
			Index = Availables.Get();
			Instance = &Objects[Index];

			Availables.Remove();
		}

		NxFr::Handle<GameObject> Handle = Handles.AcquireHandle(Instance);
		Infos.Append(GameObjectId, Info{ .Index = Index , .Handle = Handle });
		Instance->Id = GameObjectId;

		return Handle;
	}

	void World::Free(NxFr::Handle<GameObject> Instance)
	{
		NxFr::GUID GameObjectId = Instance->GetId();

		Info Info = Infos[GameObjectId];
		Infos.Remove(GameObjectId);

		Availables.Append(Info.Index);
	}

	void World::Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index)
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

	void World::Detach(NxFr::Handle<GameObject> Instance)
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
