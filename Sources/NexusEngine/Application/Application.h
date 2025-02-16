#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Application
	{
	public:
		NEXUS_ENGINE_API Application();
		NEXUS_ENGINE_API virtual ~Application();
		NEXUS_ENGINE_API virtual void Run();
	};
}

