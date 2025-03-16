#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class TimeManager
	{
		friend class Application;

	public:
		NEXUS_ENGINE_API static TimeManager* GetInstance();
		NEXUS_ENGINE_API static void Initialize();
		NEXUS_ENGINE_API static void Shutdown();

		float GetDeltaTime() const { return (float)DeltaTime; }
		uint64 GetFrameIndex() const { return FrameIndex; }

	private:
		NEXUS_ENGINE_API TimeManager();
		NEXUS_ENGINE_API ~TimeManager();

		NEXUS_ENGINE_API void Run();
		NEXUS_ENGINE_API void Tick();

	private:
		NxFr::Stopwatch Watch;

		uint64 FrameIndex;
		double DeltaTime;
	};
}
