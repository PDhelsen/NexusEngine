#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(World)

	World::World(NxFr::StringId Name, bool Reference)
		: WorldId(Name), Factory(WorldId, Reference), Root()
	{
		SetTickable(true);
	}

	World::~World()
	{
	}

	NxFr::Handle<GameObject> World::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent)
	{
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		if (!Parent && Root)
		{
			Parent = Root;
		}

		NxFr::Handle<GameObject> Instance = Factory.CreateGameObject(Name, Parent);
		Instance->Initialize();
		Instance->SetEnabled(true);

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
		return Instance;
	}

	NxFr::Handle<GameObject> World::DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent, bool Instantiate)
	{
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		if (!Parent)
		{
			Parent = Target->GetParent();
		}
		if (!Parent && Root)
		{
			Parent = Root;
		}

		NxFr::Handle<GameObject> Instance = Factory.DuplicateGameObject(Target, Parent, Instantiate);
		Instance->PatchReferences();
		Instance->Initialize();
		Instance->UpdateHierarchy();

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
		return Instance;
	}

	void World::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");

		if (!Instance)
		{
			return;
		}

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::RemovedId, WorldId, Instance->GetId());

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Factory.DestroyGameObject(Instance);
	}

	void World::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Target, int64 Index)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");
		NEXUS_ASSERT(Belong(Target), Default, "Target should belong to the same Factory");

		if (!Target)
		{
			Target = Root;
		}
		if (Index < 0)
		{
			Index = Target->GetChildCount();
		}

		Factory.AttachGameObject(Instance, Target, Index);
		Instance->UpdateHierarchy();

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
	}

	void World::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");

		Factory.DetachGameObject(Instance);
		Instance->UpdateHierarchy();

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::RemovedId, WorldId, Instance->GetId());
	}

	NxFr::Handle<Behaviour> World::CreateBehaviour(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NEXUS_ASSERT(Belong(Target), Default, "Instance should belong to the same Factory");

		NxFr::Handle<Behaviour> Instance = Factory.CreateBehaviour(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		Target->UpdateHierarchy();
		return Instance;
	}

	void World::DestroyBehaviour(NxFr::Handle<Behaviour> Instance)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");

		if (!Instance)
		{
			return;
		}

		NxFr::Handle<GameObject> Target = Instance->GetGameObject();

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Factory.DestroyBehaviour(Instance);

		Target->UpdateHierarchy();
	}

	NxFr::Handle<Component> World::CreateComponent(NxFr::StringId Type, NxFr::Handle<GameObject> Target)
	{
		NEXUS_ASSERT(Belong(Target), Default, "Instance should belong to the same Factory");

		NxFr::Handle<Component> Instance = Factory.CreateComponent(Type, Target);
		Instance->Initialize();
		Instance->SetEnabled(true);

		Target->UpdateHierarchy();
		return Instance;
	}

	void World::DestroyComponent(NxFr::Handle<Component> Instance)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");

		if (!Instance)
		{
			return;
		}

		NxFr::Handle<GameObject> Target = Instance->GetGameObject();

		Instance->SetEnabled(false);
		Instance->Shutdown();
		Factory.DestroyComponent(Instance);

		Target->UpdateHierarchy();
	}

	bool World::Belong(NxFr::Handle<GameObject> Instance) const
	{
		return Factory.Belong(Instance);
	}

	bool World::Belong(NxFr::Handle<Behaviour> Instance) const
	{
		return Factory.Belong(Instance);
	}

	bool World::Belong(NxFr::Handle<Component> Instance) const
	{
		return Factory.Belong(Instance);
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::FindGameObjects(NxFr::StringView Query) const
	{
		return Factory.FindGameObjects(Query);
	}

	NxFr::Array<NxFr::Handle<Behaviour>> World::FindBehaviours(NxFr::StringView Query) const
	{
		return Factory.FindBehaviours(Query);
	}

	NxFr::Array<NxFr::Handle<Component>> World::FindComponents(NxFr::StringView Query) const
	{
		return Factory.FindComponents(Query);
	}

	NxFr::Array<NxFr::Handle<Tags>> World::FindTags(NxFr::StringView Query, bool MatchAll)
	{
		NxFr::List<NxFr::Handle<Tags>> Result;

		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Query, " ");
		NxFr::Collection<NxFr::StringView> Collection = Filters;

		for (auto Iterator = BeginComponents<Tags>(); Iterator != EndComponents<Tags>(); ++Iterator)
		{
			if (Iterator->Contains(Collection, MatchAll))
			{
				NxFr::Handle<GameObject> Instance = Iterator->GetGameObject();
				Result.Append(Instance->GetComponentById(Iterator->GetId()));
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::Handle<Tags>>(Result);
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::GetGameObjects() const
	{
		return Factory.GetGameObjects();
	}

	NxFr::Handle<GameObject> World::GetGameObject(NxFr::GUID GameObjectId) const
	{
		if (GameObjectId == 0)
		{
			return NxFr::Handle<GameObject>();
		}

		return Factory.GetGameObject(GameObjectId);
	}

	NxFr::Handle<GameObject> World::GetRootGameObject() const
	{
		return Root;
	}

	NxFr::Array<NxFr::Handle<Behaviour>> World::GetBehaviours() const
	{
		return Factory.GetBehaviours();
	}

	NxFr::Handle<Behaviour> World::GetBehaviour(NxFr::GUID BehaviourId) const
	{
		if (BehaviourId == 0)
		{
			return NxFr::Handle<Behaviour>();
		}

		return Factory.GetBehaviour(BehaviourId);
	}

	NxFr::Array<NxFr::Handle<Component>> World::GetComponents() const
	{
		return Factory.GetComponents();
	}

	NxFr::Handle<Component> World::GetComponent(NxFr::GUID ComponentId) const
	{
		if (ComponentId == 0)
		{
			return NxFr::Handle<Component>();
		}

		return Factory.GetComponent(ComponentId);
	}

	void World::OnInitialize()
	{
		Root = CreateGameObject(WorldId);
	}

	void World::OnShutdown()
	{
		DestroyGameObject(Root);
	}

	void World::OnTick(float TimeStep)
	{
		Root->Tick();
	}
}
