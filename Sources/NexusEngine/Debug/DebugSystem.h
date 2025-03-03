#pragma once

#include "NexusEngine/Application/System.h"

namespace NxEn
{
	class DebugSystem : public System
	{
	public:
		NEXUS_SYSTEM_DECLARATION(DebugSystem);

		NEXUS_ENGINE_API virtual void OnTick() override;

	private:
		inline void TickDebugGlobals() const;
	};
}

