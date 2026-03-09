#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Jobs/Job.h"
#include "NexusEngine/Systems/Jobs/JobCompletion.h"

namespace NxEn
{
	Job::Job(JobCompletion* Completion, const NxFr::Delegate<void()>& Work)
		: Completion(Completion), Work(Work)
	{
	}

	Job::~Job()
	{
		Completion->Release();
	}

	void Job::Execute()
	{
		Work();
		Completion->Signal();
	}
}
