#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct JobCompletion
	{
	public:
		NEXUS_ENGINE_API JobCompletion(uint64 Count);
		NEXUS_ENGINE_API ~JobCompletion();

		NEXUS_ENGINE_API void Release();

		NEXUS_ENGINE_API void Signal();
		NEXUS_ENGINE_API void Wait();
		NEXUS_ENGINE_API bool IsDone() const;

	private:
		NxFr::Atomic Count;

		NxFr::Atomic Refs;
		NxFr::Mutex Guard;
		NxFr::ConditionVariable Notification;
	};
}
