#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Jobs/JobHandle.h"

namespace NxEn
{
	class JobSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, JobSystem)

		NX_ENGINE_API JobHandle Dispatch(uint64 Count, uint64 Group, NxFr::Delegate<void(uint64)> Work);
		NX_ENGINE_API JobHandle Dispatch(uint64 Count, NxFr::Delegate<void(uint64)> Work);
		NX_ENGINE_API JobHandle Submit(NxFr::Delegate<void()> Work);

		NX_ENGINE_API bool IsWorking() const;

	protected:
		NX_ENGINE_API virtual void OnInitialize() override;
		NX_ENGINE_API virtual void OnShutdown() override;

	private:
		NX_ENGINE_API void Enqueue(struct JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		NX_ENGINE_API void Worker();

	private:
		NxFr::Array<NxFr::Thread*> Threads;
		NxFr::Queue<struct Job*> Jobs;

		NxFr::Atomic Working;
		NxFr::Atomic Running;

		mutable NxFr::Mutex Guard;
		mutable NxFr::ConditionVariable Notification;
	};
}
