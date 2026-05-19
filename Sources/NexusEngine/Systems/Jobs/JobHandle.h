#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct JobHandle
	{
		friend class JobSystem;

	public:
		NX_ENGINE_API JobHandle(const JobHandle& Other) = delete;
		NX_ENGINE_API JobHandle(JobHandle&& Other) noexcept;
		NX_ENGINE_API ~JobHandle();

		NX_ENGINE_API void Wait();
		NX_ENGINE_API bool IsDone() const;

	private:
		NX_ENGINE_API JobHandle(struct JobCompletion* Completion);

	private:
		struct JobCompletion* Completion;
	};
}
