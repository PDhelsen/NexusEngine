#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	class NX_ENGINE_API DebugSystem : public System
	{
		friend class NexusEngineApplication;

	public:
		NX_OBJECT(DebugSystem)

		DebugSystem();
		~DebugSystem();

		NxFr::Logger* GetLogger() const { return Logs; }
		NxFr::Stats* GetStats() const { return Statistiques; };
		NxFr::Instruments* GetInstruments() const { return Instrumentor; }
		NxFr::MemoryTracker* GetMemory() const { return Memory; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void AutoStart();
		void ApplySettings();

		TimeManager* Time;
		NxFr::Logger* Logs;
		NxFr::Stats* Statistiques;
		NxFr::Instruments* Instrumentor;
		NxFr::MemoryTracker* Memory;
	};
}
