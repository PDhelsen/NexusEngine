#pragma once

#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	class DebugSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, DebugSystem)

	protected:
		NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;

	private:
		inline void TickDebugGlobals() const;

	private:
		uint64 FrameCount;
	};
}

