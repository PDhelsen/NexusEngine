#pragma once

#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	class DebugSystem : public System
	{
		friend class NexusEngineApplication;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, DebugSystem)

		NX_ENGINE_API DebugSystem();
		NX_ENGINE_API ~DebugSystem();

		NX_ENGINE_API NxFr::Logger* GetLogger() const { return Logger; }
		NX_ENGINE_API NxFr::Stats* GetStats() const { return Stats; };
		NX_ENGINE_API NxFr::Instruments* GetInstrumentor() const { return Instrumentor; }
		NX_ENGINE_API NxFr::MemoryTracker* GetMemory() const { return Memory; }

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NX_ENGINE_API void AutoStart();
		NX_ENGINE_API void RecordStats();
		NX_ENGINE_API void TickProfiler();
		NX_ENGINE_API void ApplySettings();

	private:
		NxFr::Logger* Logger;
		NxFr::Stats* Stats;
		NxFr::Instruments* Instrumentor;
		NxFr::MemoryTracker* Memory;
		double Time;
	};
}
