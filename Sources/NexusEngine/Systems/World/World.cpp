#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/World.h"

namespace NxEn
{
	World::World()
		: WorldId(0), Root()
	{
		SetTickable(true);
	}

	World::~World()
	{
	}

	NxFr::Handle<GameObject> World::GetRoot() const
	{
		return Root;
	}

	NxFr::GUID World::GetId() const
	{
		return WorldId;
	}

	NxFr::GUID World::GetWorldId() const
	{
		return WorldId;
	}

	NxFr::StringView World::GetName() const
	{
		return WorldId.GetString();
	}

	void World::OnTick(float TimeStep)
	{
		Root->Tick(TimeStep);
	}
}
