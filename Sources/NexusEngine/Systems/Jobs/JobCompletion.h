#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct NX_ENGINE_API JobCompletion
	{
	public:
		NX_NOCOPY_NOMOVE(JobCompletion);
		JobCompletion(uint64 Count);
		~JobCompletion();
		void Release();

		void Signal();
		void Wait();
		bool IsDone() const;

	private:
		NxFr::Atomic Count;
		NxFr::Atomic Refs;
		NxFr::Mutex Guard;
		NxFr::ConditionVariable Notification;
	};
}
