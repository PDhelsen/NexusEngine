#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/TimeManager.h"

namespace NxEn
{
	TimeManager::TimeManager()
		: Watch(), FrameIndex(0), DeltaTime(0.0f), UnscaledDeltaTime(0.0f), Time(0.0f), UnscaledTime(0.0f), Multiplier(1.0f)
	{
	}

	TimeManager::~TimeManager()
	{
	}

	void TimeManager::Run()
	{
		Watch.Start();
	}

	void TimeManager::Tick()
	{
		UnscaledDeltaTime = Watch.Stop();
		UnscaledTime += UnscaledDeltaTime;
		Watch.Start();

		DeltaTime = UnscaledDeltaTime * Multiplier;
		Time += DeltaTime;

		FrameIndex++;
	}
}
