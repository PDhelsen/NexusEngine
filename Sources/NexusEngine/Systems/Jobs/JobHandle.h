#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct JobHandle
	{
		friend class JobSystem;

	public:
		NEXUS_ENGINE_API JobHandle(const JobHandle& Other) = delete;
		NEXUS_ENGINE_API JobHandle(JobHandle&& Other) noexcept;
		NEXUS_ENGINE_API ~JobHandle();

		NEXUS_ENGINE_API void Wait();
		NEXUS_ENGINE_API bool IsDone() const;

	private:
		NEXUS_ENGINE_API JobHandle(struct JobCompletion* Completion);

	private:
		struct JobCompletion* Completion;
	};
}
