#pragma once

#include "NexusEngine/Systems/World/World/World.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectFactory.h"

namespace NxEn
{
	struct WorldObjectFactoryContext
	{
	public:
		NX_ENGINE_API static WorldObjectFactory* GetFactory();

		NX_ENGINE_API WorldObjectFactoryContext(World* Instance);
		NX_ENGINE_API ~WorldObjectFactoryContext();
	};
}

