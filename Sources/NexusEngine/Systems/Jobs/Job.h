#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct Job
	{
	public:
		NEXUS_ENGINE_API Job(struct JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		NEXUS_ENGINE_API ~Job();

		NEXUS_ENGINE_API void Execute();

	private:
		struct JobCompletion* Completion;
		NxFr::Delegate<void()> Work;
	};
}
