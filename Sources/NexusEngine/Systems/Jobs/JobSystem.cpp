#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Jobs/JobSystem.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"

namespace NxEn
{
	static NxEn::SettingVar<uint64>* SettingThreadCounts = NxEn::SettingVar<uint64>::Create("Settings", "JobReservedThread", 4);

	JobHandle JobSystem::Dispatch(uint64 Count, uint64 Group, NxFr::Delegate<void(uint64)> Work)
	{
		uint64 Batch = (Count + Group - 1) / Group;
		JobCompletion* Completion = new JobCompletion(Batch);

		for (uint64 Index = 0; Index < Batch; ++Index)
		{
			Enqueue(Completion, [Work, Index, Count, Group]()
			{
				uint64 Begin = Index * Group;
				uint64 End = NxFr::Math::Min(Begin + Group, Count);
				for (uint64 It = Begin; It < End; ++It)
				{
					Work(It);
				}
			});
		}

		return Completion;
	}

	JobHandle JobSystem::Dispatch(uint64 Count, NxFr::Delegate<void(uint64)> Work)
	{
		JobCompletion* Completion = new JobCompletion(Count);

		for (uint64 Index = 0; Index < Count; ++Index)
		{
			Enqueue(Completion, [Work, Index]()
			{
				Work(Index);
			});
		}

		return Completion;
	}

	JobHandle JobSystem::Submit(const NxFr::Delegate<void()>& Work)
	{
		JobCompletion* Completion = new JobCompletion(1);

		Enqueue(Completion, Work);

		return Completion;
	}

	bool JobSystem::IsWorking() const
	{
		NxFr::Lock GuardLock(Guard);
		return Working > 0 || !Jobs.IsEmpty();
	}

	void JobSystem::OnInitialize()
	{
		uint64 MaxThreads = NxFr::ThreadPool::MaxThreadCount();
		uint64 Reservered = SettingThreadCounts->GetValue();
		uint64 ThreadsCount = MaxThreads > Reservered ? MaxThreads - Reservered : 1llu;

		Running = true;
		Threads = ThreadsCount;

		for (uint64 Index = 0; Index < Threads.GetCount(); ++Index)
		{
			Threads[Index] = new NxFr::Thread({ this, &JobSystem::Worker });
			Threads[Index]->Run();
		}
	}

	void JobSystem::OnShutdown()
	{
		{
			NxFr::Lock GuardLock(Guard);
			Running = false;
		}

		Notification.Broadcast();

		for (uint64 Index = 0; Index < Threads.GetCount(); ++Index)
		{
			Threads[Index]->Join();
			delete Threads[Index];
		}
	}

	void JobSystem::Enqueue(JobCompletion* Completion, const NxFr::Delegate<void()>& Work)
	{
		NxFr::Lock GuardLock(Guard);

		Job* Instance = new Job(Completion, Work);
		Jobs.Append(Instance);
		Notification.Signal();
	}

	void JobSystem::Worker()
	{
		while (true)
		{
			Job* Instance;

			// Wait & Fetch
			{
				NxFr::Lock GuardLock(Guard);

				Notification.Wait(Guard, [&]() { return !Running || !Jobs.IsEmpty(); });
				if (!Running && Jobs.IsEmpty())
				{
					return;
				}

				Instance = Jobs.Get();
				Jobs.Remove();

				Working++;
			}

			// Execute
			Instance->Execute();
			delete Instance;

			// Notify
			{
				NxFr::Lock GuardLock(Guard);
				Working--;
			}
		}
	}
}
