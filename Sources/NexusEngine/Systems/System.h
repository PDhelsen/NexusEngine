#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class System : public Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, System)

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize() override;
		NEXUS_ENGINE_API virtual void OnShutdown() override;
		NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
	};
}
