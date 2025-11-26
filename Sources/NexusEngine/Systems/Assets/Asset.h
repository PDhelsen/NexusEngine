#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class Asset : public Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Asset)

	private:
		NEXUS_ENGINE_API Asset();
		NEXUS_ENGINE_API ~Asset();

		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnDisable() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep) override;

	private:
	};
}
