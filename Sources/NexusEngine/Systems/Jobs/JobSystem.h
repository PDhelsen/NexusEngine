#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Jobs/JobHandle.h"

namespace NxEn
{
	class NX_ENGINE_API JobSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(JobSystem)

		JobHandle Dispatch(uint64 Count, uint64 Group, NxFr::Delegate<void(uint64)> Work);
		JobHandle Dispatch(uint64 Count, NxFr::Delegate<void(uint64)> Work);
		JobHandle Submit(NxFr::Delegate<void()> Work);

		bool IsWorking() const;

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;

	private:
		void Enqueue(struct JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		void Worker();

	private:
		NxFr::Array<NxFr::Thread*> Threads;
		NxFr::Queue<struct Job*> Jobs;

		NxFr::Atomic Working;
		NxFr::Atomic Running;

		mutable NxFr::Mutex Guard;
		mutable NxFr::ConditionVariable Notification;
	};
}
