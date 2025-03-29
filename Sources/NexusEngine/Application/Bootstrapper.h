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

		NEXUS_ENGINE_API Bootstrapper& AppendStep(const Signature& Step, NxFr::StringView Tag = "");

		template<typename T, typename D>
		Bootstrapper& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }
		NEXUS_ENGINE_API Bootstrapper& AppendSystem(System* Target);
		NEXUS_ENGINE_API Bootstrapper& AppendDependency(NxFr::StringId Target, NxFr::StringId Dependency);

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
		NxFr::Logger Logger;
	};
}
