#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class AssetsSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, AssetsSystem)

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
	};
}
