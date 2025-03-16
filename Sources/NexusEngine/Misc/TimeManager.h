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

		void SetMultiplier(float Mult) { Multiplier = Mult; }

		uint64 GetFrameIndex() const { return FrameIndex; }
		float GetDeltaTime() const { return (float)DeltaTime; }
		float GetUnscaledDeltaTime() const { return (float)UnscaledDeltaTime; }
		float GetTime() const { return (float)Time; }
		float GetUnscaledTime() const { return (float)UnscaledTime; }
		float GetMultiplier() const { return Multiplier; }

	private:
		NEXUS_ENGINE_API TimeManager();
		NEXUS_ENGINE_API ~TimeManager();

		NEXUS_ENGINE_API void Run();
		NEXUS_ENGINE_API void Tick();

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
