#pragma once

#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	class NX_ENGINE_API DebugSystem : public System
	{
		friend class NexusEngineApplication;

	public:
		NX_OBJECT_DECLARATION(DebugSystem)

		DebugSystem();
		~DebugSystem();

		NxFr::Logger* GetLogger() const { return Logger; }
		NxFr::Stats* GetStats() const { return Stats; };
		NxFr::Instruments* GetInstrumentor() const { return Instrumentor; }
		NxFr::MemoryTracker* GetMemory() const { return Memory; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void AutoStart();
		void RecordStats();
		void TickProfiler();
		void ApplySettings();

	private:
		NxFr::Logger* Logger;
		NxFr::Stats* Stats;
		NxFr::Instruments* Instrumentor;
		NxFr::MemoryTracker* Memory;
		double Time;
	};
}
