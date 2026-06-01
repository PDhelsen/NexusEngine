#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Jobs/JobCompletion.h"

namespace NxEn
{
	struct NX_ENGINE_API Job
	{
	public:
		NX_NOCOPY_NOMOVE(Job);
		Job(JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		~Job();

		void Execute();

	private:
		JobCompletion* Completion;
		NxFr::Delegate<void()> Work;
	};
}
