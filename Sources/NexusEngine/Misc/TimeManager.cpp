#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/TimeManager.h"

namespace NxEn
{
	static TimeManager* Instance = nullptr;

	TimeManager* TimeManager::GetInstance()
	{
		return Instance;
	}

	void TimeManager::Initialize()
	{
		NEXUS_ASSERT(Instance == nullptr, Default, "TimeManager was already created");
		Instance = new TimeManager();
	}

	void TimeManager::Shutdown()
	{
		delete Instance;
		Instance = nullptr;
	}

	TimeManager::TimeManager()
		: Watch(), FrameIndex(0), DeltaTime(0.0f), UnscaledDeltaTime(0.0f), Time(0.0f), UnscaledTime(0.0f), Multiplier(1.0f)
	{
	}

	TimeManager::~TimeManager()
	{
		NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)UnscaledTime)
	}

	void TimeManager::Run()
	{
		Watch.Start();
	}

	void TimeManager::Tick()
	{
		UnscaledDeltaTime = Watch.Stop(NxFr::Time::SecondToMilli);
		UnscaledTime += UnscaledDeltaTime * NxFr::Time::MilliToSecond;
		Watch.Start();

		DeltaTime = UnscaledDeltaTime * Multiplier;
		Time += DeltaTime * NxFr::Time::MilliToSecond;

		FrameIndex++;
	}
}
