#pragma once

#include "NexusEngine/Application/System.h"

namespace NxEn
{
	class DebugSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(DebugSystem)

	protected:
		NEXUS_ENGINE_API virtual void OnTick() override;

	private:
		inline void TickDebugGlobals() const;
	};
}

