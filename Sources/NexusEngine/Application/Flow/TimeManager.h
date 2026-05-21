#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class NX_ENGINE_API TimeManager
	{
	public:
		TimeManager();
		~TimeManager();

		void Run();
		void Tick();

		uint64 GetFrameIndex() const { return FrameIndex; }
		float GetDeltaTime() const { return (float)DeltaTime; }
		float GetUnscaledDeltaTime() const { return (float)UnscaledDeltaTime; }
		float GetTime() const { return (float)Time; }
		float GetUnscaledTime() const { return (float)UnscaledTime; }
		void SetMultiplier(float Mult) { Multiplier = Mult; }
		float GetMultiplier() const { return Multiplier; }

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
