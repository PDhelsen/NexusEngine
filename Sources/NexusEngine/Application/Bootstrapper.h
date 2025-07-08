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

		NEXUS_ENGINE_API Bootstrapper& AppendStep(const Signature& Step, NxFr::StringView Tag = "Step");

		template<typename T>
		Bootstrapper& AppendSystem() { return AppendSystem(T::GetClassType()); }
		template<typename T, typename D>
		Bootstrapper& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }

		NEXUS_ENGINE_API Bootstrapper& AppendSystem(NxFr::StringId Type);
		NEXUS_ENGINE_API Bootstrapper& AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency);

		uint64 GetStepsCount() const { return Steps.GetCount(); }
		uint64 GetSystemsCount() const { return Systems.GetCount(); }

	private:
		void RunBoot();
		void RunUnboot();

		void ExecuteSteps(bool Boot);
		void ExecuteSystems(bool Boot);

	private:
		NxFr::List<NxFr::Tuple<Signature, NxFr::StringView>> Steps;
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> Systems;
		NxFr::Logger Logger;
	};
}
