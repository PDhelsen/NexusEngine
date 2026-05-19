#pragma once

#include "NexusEngine/Systems/World/World/World.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

namespace NxEn
{
	struct WorldObjectFactoryContext
	{
	public:
		static WorldObjectFactory* GetFactory();

		WorldObjectFactoryContext(World* Instance);
		~WorldObjectFactoryContext();
	};
}

