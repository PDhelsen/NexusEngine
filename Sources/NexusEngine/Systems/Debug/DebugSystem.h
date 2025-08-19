#pragma once

#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	class DebugSystem : public System
	{
		friend class NexusEngineApplication;

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

		void Start();
		void Stop();
		void Flush();

	private:
		NxFr::Logger* Logger;
		NxFr::Stats* Stats;
		NxFr::Instruments* Instrumentor;
	};
}
