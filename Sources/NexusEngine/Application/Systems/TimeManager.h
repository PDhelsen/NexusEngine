#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class TimeManager
	{
	public:
		NEXUS_ENGINE_API TimeManager();
		NEXUS_ENGINE_API ~TimeManager();

		NEXUS_ENGINE_API void Run();
		NEXUS_ENGINE_API void Tick();

		NEXUS_ENGINE_API void SetMultiplier(float Mult) { Multiplier = Mult; }

		NEXUS_ENGINE_API uint64 GetFrameIndex() const { return FrameIndex; }
		NEXUS_ENGINE_API float GetDeltaTime() const { return (float)DeltaTime; }
		NEXUS_ENGINE_API float GetUnscaledDeltaTime() const { return (float)UnscaledDeltaTime; }
		NEXUS_ENGINE_API float GetTime() const { return (float)Time; }
		NEXUS_ENGINE_API float GetUnscaledTime() const { return (float)UnscaledTime; }
		NEXUS_ENGINE_API float GetMultiplier() const { return Multiplier; }

	private:
		NxFr::Stopwatch Watch;

		uint64 FrameIndex;
		double DeltaTime;
		double UnscaledDeltaTime;
		double Time;
		double UnscaledTime;
		float Multiplier;
	};
}
