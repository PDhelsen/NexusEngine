#pragma once

#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"
#include "NexusEngine/Systems/World/World/World.h"

namespace NxEn
{
	struct WorldObjectFactoryContext
	{
	public:
		NEXUS_ENGINE_API static WorldObjectFactory* GetFactory();

		NEXUS_ENGINE_API WorldObjectFactoryContext(World* Instance);
		NEXUS_ENGINE_API ~WorldObjectFactoryContext();
	};
}

