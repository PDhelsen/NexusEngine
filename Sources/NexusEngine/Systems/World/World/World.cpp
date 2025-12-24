#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(World)

	World::World(NxFr::GUID WorldId, NxFr::StringView Name)
		: WorldId(WorldId), Name(Name), Factory(WorldId, false), Root()
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
		Instance->Initialize();
		Instance->SetEnabled(Target->IsEnabled());

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

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
	}

	void World::DetachGameObject(NxFr::Handle<GameObject> Instance)
	{
		NEXUS_ASSERT(Belong(Instance), Default, "Instance should belong to the same Factory");

		Factory.DetachGameObject(Instance);

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::RemovedId, WorldId, Instance->GetId());
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
		Root = CreateGameObject("Root");
	}

	void World::OnShutdown()
	{
		DestroyGameObject(Root);
	}

	void World::OnTick(float TimeStep)
	{
		Root->Tick(TimeStep);
	}
}
