#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class TimeManager
	{
	public:
		NX_ENGINE_API TimeManager();
		NX_ENGINE_API ~TimeManager();

		NX_ENGINE_API void Run();
		NX_ENGINE_API void Tick();

		NX_ENGINE_API void SetMultiplier(float Mult) { Multiplier = Mult; }

		NX_ENGINE_API uint64 GetFrameIndex() const { return FrameIndex; }
		NX_ENGINE_API float GetDeltaTime() const { return (float)DeltaTime; }
		NX_ENGINE_API float GetUnscaledDeltaTime() const { return (float)UnscaledDeltaTime; }
		NX_ENGINE_API float GetTime() const { return (float)Time; }
		NX_ENGINE_API float GetUnscaledTime() const { return (float)UnscaledTime; }
		NX_ENGINE_API float GetMultiplier() const { return Multiplier; }

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
