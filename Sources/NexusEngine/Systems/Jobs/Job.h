#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct NX_ENGINE_API Job
	{
	public:
		Job(struct JobCompletion* Completion, const NxFr::Delegate<void()>& Work);
		~Job();

		void Execute();

	private:
		struct JobCompletion* Completion;
		NxFr::Delegate<void()> Work;
	};
}
