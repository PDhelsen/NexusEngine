#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	const static NxEn::Command CmdWorldInstantiate = NxEn::Command::Create("World.Instantiate"_Sid, "Instantiate into the Main world", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* System = Application::GetSystem<AssetsSystem>();
		NxFr::GUID Id = System->PathToId(Path);
		Prefab* Instance = System->Acquire<Prefab>(Id);
		Application::GetSystem<WorldSystem>()->GetWorld()->Instantiate(*Instance);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(World)

	World::World(NxFr::GUID WorldId)
		: WorldId(WorldId), Name(), Factory(WorldId, ObjectFactory::ReferenceMode::Bake), Root()
	{
		SetTickable(true);
	}

	World::~World()
	{
	}

	NxFr::Handle<GameObject> World::Instantiate(const Prefab& Target, NxFr::Handle<GameObject> Parent)
	{
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		if (!Parent && Root)
		{
			Parent = Root;
		}

		NxFr::Handle<GameObject> Instance = Factory.Instantiate(Target, Parent);
		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
		return Instance;
	}

	NxFr::Handle<GameObject> World::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent)
	{
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		if (!Parent && Root)
		{
			Parent = Root;
		}

		NxFr::Handle<GameObject> Instance = Factory.CreateGameObject(Name, 0, Parent);
		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
		return Instance;
	}

	NxFr::Handle<GameObject> World::DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent)
	{
		NEXUS_ASSERT(Belong(Target), Default, "Target should belong to the same Factory");
		NEXUS_ASSERT(!Parent || Belong(Parent), Default, "Parent should belong to the same Factory");

		if (!Parent)
		{
			Parent = Target->GetParent();
		}

		NxFr::Handle<GameObject> Instance = Factory.DuplicateGameObject(Target, Parent);
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
		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::ChangedId, WorldId, Instance->GetId());
	}

	bool World::Belong(NxFr::Handle<GameObject> Instance) const
	{
		return Instance->GetWorldId() == WorldId;
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::Find(NxFr::StringView Filter) const
	{
		return Factory.Find(Filter);
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::GetGameObjects() const
	{
		return Factory.GetGameObjects();
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::GetPrefabs() const
	{
		return Factory.GetPrefabs();
	}

	NxFr::Handle<GameObject> World::GetGameObject(NxFr::GUID GameObjectId) const
	{
		return Factory.GetGameObject(GameObjectId);
	}

	NxFr::Handle<GameObject> World::GetRootGameObject() const
	{
		return Root;
	}

	void World::OnInitialize()
	{
		Root = CreateGameObject("");
	}

	void World::OnTick(float TimeStep)
	{
		for (auto It = Factory.IteratorGameObjectBegin(); It != Factory.IteratorGameObjectEnd(); ++It)
		{
			It->Tick(TimeStep);
		}
	}
}
