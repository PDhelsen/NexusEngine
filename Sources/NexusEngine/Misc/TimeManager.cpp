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
		: Watch(), FrameIndex(0), DeltaTime(0)
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
		DeltaTime = Watch.Stop(NxFr::Time::SecondToMilli);
		Watch.Start();

		FrameIndex++;
	}
}
