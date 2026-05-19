#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct NX_ENGINE_API JobHandle
	{
		friend class JobSystem;

	public:
		JobHandle(const JobHandle& Other) = delete;
		JobHandle(JobHandle&& Other) noexcept;
		~JobHandle();

		void Wait();
		bool IsDone() const;

	private:
		JobHandle(struct JobCompletion* Completion);

	private:
		struct JobCompletion* Completion;
	};
}
