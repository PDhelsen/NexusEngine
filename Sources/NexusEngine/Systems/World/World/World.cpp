#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(World)

	World::World(NxFr::GUID WorldId)
		: WorldId(WorldId), Factory(WorldId)
	{
		SetTickable(true);
	}

	World::~World()
	{
	}

	NxFr::Handle<GameObject> World::GetGameObject(NxFr::GUID GameObjectId)
	{
		return Factory.GetGameObject(GameObjectId);
	}

	NxFr::Handle<GameObject> World::CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent)
	{
		NxFr::Handle<GameObject> Instance = Factory.CreateGameObject(Name, Parent);
		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
		return Instance;
	}

	NxFr::Handle<GameObject> World::DuplicateGameObject(NxFr::Handle<GameObject> Target, NxFr::Handle<GameObject> Parent)
	{
		NxFr::Handle<GameObject> Instance = Factory.DuplicateGameObject(Target, Parent);
		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::AppendedId, WorldId, Instance->GetId());
		return Instance;
	}

	void World::DestroyGameObject(NxFr::Handle<GameObject> Instance)
	{
		if (!Instance)
		{
			return;
		}

		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::RemovedId, WorldId, Instance->GetId());
		Factory.DestroyGameObject(Instance);
	}

	void World::AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Target, int64 Index)
	{
		Factory.AttachGameObject(Instance, Target, Index);
		Application::GetSystem<WorldSystem>()->GetOnGameObjectEvent().Invoke(WorldSystem::ChangedId, WorldId, Instance->GetId());
	}

	bool World::Belong(NxFr::Handle<GameObject> Instance)
	{
		return Instance->GetWorldId() == WorldId;
	}

	NxFr::Array<NxFr::Handle<GameObject>> World::Find(NxFr::StringView Filter)
	{
		return Factory.Find(Filter);
	}

	uint64 World::GetGameObjectsCount() const
	{
		return Factory.GetGameObjectsCount();
	}

	NxFr::Handle<GameObject> World::GetRootGameObject()
	{
		return Factory.GetRootGameObject();
	};

	void World::OnTick(float TimeStep)
	{
		NxFr::List<GameObject>& GameObjects = Factory.GetGameObjects();
		for (auto& Instance : GameObjects)
		{
			Instance.Tick(TimeStep);
		}
	}
}
