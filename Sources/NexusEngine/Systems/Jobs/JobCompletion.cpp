#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Jobs/JobCompletion.h"

namespace NxEn
{
	JobCompletion::JobCompletion(uint64 Count)
		: Count(Count), Refs(Count + 1), Guard(), Notification()
	{
	}

	JobCompletion::~JobCompletion()
	{
	}

	void JobCompletion::Release()
	{
		if (Refs.Decrement() == 0)
		{
			delete this;
		}
	}

	void JobCompletion::Signal()
	{
		if (Count.Decrement() == 0)
		{
			NxFr::Lock LockGuard(Guard);
			Notification.Broadcast();
		}
	}

	void JobCompletion::Wait()
	{
		NxFr::Lock LockGuard(Guard);
		Notification.Wait(Guard, [&]() { return IsDone(); });
	}

	bool JobCompletion::IsDone() const
	{
		return Count.Load() == 0;
	}
}
