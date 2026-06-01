#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Jobs/Job.h"
#include "NexusEngine/Systems/Jobs/JobHandle.h"
#include "NexusEngine/Systems/Jobs/JobCompletion.h"

namespace NxEn
{
	class NX_ENGINE_API JobSystem : public System
	{
	public:
		NX_OBJECT(JobSystem)

		JobHandle Dispatch(uint64 Count, uint64 Group, NxFr::Delegate<void(uint64)> Work);
		JobHandle Dispatch(uint64 Count, NxFr::Delegate<void(uint64)> Work);
		JobHandle Submit(const NxFr::Delegate<void()>& Work);

		bool IsWorking() const;

	protected:
		void OnInitialize() override;
		void OnShutdown() override;

	private:
		void Enqueue(JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		void Worker();

		NxFr::Array<NxFr::Thread*> Threads;
		NxFr::Queue<Job*> Jobs;
		uint64 Working;
		bool Running;
		mutable NxFr::Mutex Guard;
		mutable NxFr::ConditionVariable Notification;
	};
}
