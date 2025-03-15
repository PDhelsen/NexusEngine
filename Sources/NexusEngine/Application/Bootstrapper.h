#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/SystemManager.h"

namespace NxEn
{
	class Bootstrapper
	{
		friend class Application;

	public:
		using Signature = NxFr::Delegate<void()>;

		NEXUS_ENGINE_API Bootstrapper();
		NEXUS_ENGINE_API ~Bootstrapper();

		template<typename T, typename D>
		Bootstrapper& AddDependency() { return AddDependency(T::GetClassType(), D::GetClassType()); }

		NEXUS_ENGINE_API Bootstrapper& AddStep(Bootstrapper::Signature Step, NxFr::StringView Tag = "");
		NEXUS_ENGINE_API Bootstrapper& AddSystem(System* Target);
		NEXUS_ENGINE_API Bootstrapper& AddDependency(NxFr::StringId Target, NxFr::StringId Dependency);

		uint64 GetStepsCount() const { return Steps.GetCount(); }
		uint64 GetSystemsCount() const { return Systems.GetCount(); }

	private:
		void RunBoot(const SystemManager& Manager);
		void RunUnboot(const SystemManager& Manager);

		void ExecuteSteps();
		void ExecuteSystemsInitialize(const SystemManager& Manager);
		void ExecuteSystemsShutdown(const SystemManager& Manager);

	private:
		NxFr::List<NxFr::Tuple<Signature, NxFr::StringView>> Steps;
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> Systems;
	};
}
