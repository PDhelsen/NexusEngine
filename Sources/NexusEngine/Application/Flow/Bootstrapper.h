#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/SystemManager.h"

namespace NxEn
{
	class NX_ENGINE_API Bootstrapper
	{
		friend class Application;

	public:
		using Signature = NxFr::Delegate<void()>;

		enum class StepBucket
		{
			BeforeSystem, AfterSystem
		};

	private:
		struct StepInfo
		{
			StepBucket Bucket;
			Signature Callback;
			NxFr::StringView Tag;
		};

	public:
		Bootstrapper();
		~Bootstrapper();

		Bootstrapper& AppendStep(StepBucket Bucket, NxFr::StringView Tag, const Signature& Step);

		template<typename T>
		Bootstrapper& AppendSystem() { return AppendSystem(T::GetClassType()); }
		template<typename T, typename D>
		Bootstrapper& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }

		Bootstrapper& AppendSystem(NxFr::StringId Type);
		Bootstrapper& AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency);

		uint64 GetStepsCount() const { return Steps.GetCount(); }
		uint64 GetSystemsCount() const { return Systems.GetCount(); }

	private:
		void RunBoot();
		void RunUnboot();

		void ExecuteSteps(bool Boot, StepBucket Bucket);
		void ExecuteSystems(bool Boot);

	private:
		NxFr::List<StepInfo> Steps;
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> Systems;
	};
}
