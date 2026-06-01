#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Jobs/JobHandle.h"

namespace NxEn
{
	JobHandle::JobHandle(JobCompletion* Completion)
		: Completion(Completion)
	{
	}

	JobHandle::JobHandle(JobHandle&& Other) noexcept
		: Completion(Other.Completion)
	{
		Other.Completion = nullptr;
	}

	JobHandle::~JobHandle()
	{
		if (!Completion)
		{
			return;
		}

		Completion->Release();
	}

	void JobHandle::Wait()
	{
		if (!Completion)
		{
			return;
		}

		Completion->Wait();
	}

	bool JobHandle::IsDone() const
	{
		if (!Completion)
		{
			return false;
		}

		return Completion->IsDone();
	}
}
