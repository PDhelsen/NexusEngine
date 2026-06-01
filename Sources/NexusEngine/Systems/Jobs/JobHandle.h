#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Jobs/JobCompletion.h"

namespace NxEn
{
	struct NX_ENGINE_API JobHandle
	{
	public:
		NX_NOCOPY(JobHandle);
		JobHandle(JobCompletion* Completion);
		JobHandle(JobHandle&& Other) noexcept;
		~JobHandle();

		void Wait();
		bool IsDone() const;

	private:
		JobCompletion* Completion;
	};
}
