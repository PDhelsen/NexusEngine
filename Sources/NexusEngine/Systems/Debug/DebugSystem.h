#pragma once

#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	namespace StatsHeader
	{
		NEXUS_ENGINE_API extern const NxFr::StringId GuiElementsId;
		NEXUS_ENGINE_API extern const NxFr::StringId MemoryAllocatedId;
		NEXUS_ENGINE_API extern const NxFr::StringId MemoryAllocationId;
		NEXUS_ENGINE_API extern const NxFr::StringId PlatformMemoryId;
	}

	class DebugSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, DebugSystem)

		NEXUS_ENGINE_API DebugSystem();
		NEXUS_ENGINE_API ~DebugSystem();

		NEXUS_ENGINE_API NxFr::Logger* GetLogger() const { return Logger; }
		NEXUS_ENGINE_API NxFr::Stats* GetStats() const { return Stats; };
		NEXUS_ENGINE_API NxFr::Instruments* GetInstrumentor() const { return Instrumentor; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Logger* Logger;
		NxFr::Stats* Stats;
		NxFr::Instruments* Instrumentor;
	};
}
