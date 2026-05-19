#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct Job
	{
	public:
		NX_ENGINE_API Job(struct JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		NX_ENGINE_API ~Job();

		NX_ENGINE_API void Execute();

	private:
		struct JobCompletion* Completion;
		NxFr::Delegate<void()> Work;
	};
}
