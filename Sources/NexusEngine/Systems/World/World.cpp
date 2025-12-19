#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(World)

	World::World(NxFr::StringId Id)
		: Id(Id), Name(Id.GetString()), Objects(256), Root(nullptr)
	{
		SetTickable(true);
	}

	World::~World()
	{
	}

	GameObject* World::GetGameObject(NxFr::GUID Id)
	{
		uint64* Index = Ids.TryGet(Id);
		return Index ? &Objects[*Index] : nullptr;
	}

	GameObject* World::CreateGameObject(NxFr::StringView Name, GameObject* Parent)
	{
		GameObject* Instance = &Objects.AppendConstruct(Id, Name);
		Ids.Append(Instance->GetId(), Objects.GetCount() - 1);
		Instance->Initialize();

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

	GameObject* World::DuplicateGameObject(GameObject* Instance, GameObject* Parent)
	{
		if (Parent == nullptr)
		{
			Parent = Instance->GetParent();
		}

		GameObject* Copy = CreateGameObject(Instance->GetName(), Parent);
		GameObject* Child = Instance->GetChild();
		while (Child)
		{
			DuplicateGameObject(Child, Copy);
			Child = Child->GetNext();
		}

		return Copy;
	}

	void World::DestroyGameObject(GameObject* Instance)
	{
		if (!Instance)
		{
			return;
		}

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::RemovedId, Id, Instance->GetId());

		Detach(Instance);
		Instance->Shutdown();
	}

	void World::AttachGameObject(GameObject* Instance, GameObject* Target, int64 Index)
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

	bool World::Belong(GameObject* Instance)
	{
		return Instance->WorldId == Id;
	}

	NxFr::Array<GameObject*> World::Find(NxFr::StringView Filter)
	{
		NxFr::List<GameObject*> Result;

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
				Result.Append(&Instance);
			}
		}

		return NxFr::ContainersUtils::ToArray<GameObject*>(Result);
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

	void World::Attach(GameObject* Instance, GameObject* Parent, uint64 Index)
	{
		Instance->Parent = Parent;

		if (!Parent->Child)
		{
			Parent->Child = Instance;
		}
		else
		{
			GameObject* Target = Parent->Child;
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

	void World::Detach(GameObject* Instance)
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

		Instance->Parent = nullptr;
		Instance->Prev = nullptr;
		Instance->Next = nullptr;
	}
}
