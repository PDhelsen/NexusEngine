#pragma once

#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	class GUISystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, GUISystem)

		NEXUS_ENGINE_API GUISystem();
		NEXUS_ENGINE_API ~GUISystem();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
	};
}
