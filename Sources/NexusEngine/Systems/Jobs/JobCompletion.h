#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct JobCompletion
	{
	public:
		NX_ENGINE_API JobCompletion(uint64 Count);
		NX_ENGINE_API ~JobCompletion();

		NX_ENGINE_API void Release();

		NX_ENGINE_API void Signal();
		NX_ENGINE_API void Wait();
		NX_ENGINE_API bool IsDone() const;

	private:
		NxFr::Atomic Count;

		NxFr::Atomic Refs;
		NxFr::Mutex Guard;
		NxFr::ConditionVariable Notification;
	};
}
