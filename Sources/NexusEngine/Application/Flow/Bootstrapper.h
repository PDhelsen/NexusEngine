#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class System;

	class NX_ENGINE_API Bootstrapper
	{
	public:
		using Signature = NxFr::Delegate<void()>;

		enum class BootBucket
		{
			BeforeSystem,
			AfterSystem
		};

		Bootstrapper();
		~Bootstrapper();

		void RunBoot();
		void RunUnboot();

		Bootstrapper& AppendStep(BootBucket Bucket, NxFr::StringView Tag, const Signature& Step);
		template<typename T>
		Bootstrapper& AppendSystem() { return AppendSystem(T::GetClassType()); }
		Bootstrapper& AppendSystem(NxFr::StringId Type);
		template<typename T, typename D>
		Bootstrapper& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }
		Bootstrapper& AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency);

	private:
		struct StepInfo
		{
			BootBucket Bucket;
			NxFr::String Tag;
			Signature Callback;
		};

		void ExecuteSteps(BootBucket Bucket);
		void ExecuteSystems(const NxFr::Delegate<void(System*)>& Callback);

		NxFr::List<StepInfo> Steps;
		NxFr::Dictionary<NxFr::StringId, NxFr::Set<NxFr::StringId>> Systems;
	};
}
