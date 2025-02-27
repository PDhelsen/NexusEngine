#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/System.h"

namespace NxEn
{
	class Bootstrapper
	{
		friend class Application;

		struct SystemInfo
		{
		public:
			System* Target;
			uint64 Remaining;
			NxFr::List<NxFr::StringView> Dependencies;
			NxFr::List<NxFr::StringView> Dependents;

			NEXUS_ENGINE_API SystemInfo(System* Target);
		};

	public:
		NEXUS_ENGINE_API Bootstrapper();
		NEXUS_ENGINE_API ~Bootstrapper();

		Bootstrapper& AddStep(NxFr::Delegate<void()> Step)
		{
			Steps += Step;
			return *this;
		}

		template<typename S>
		Bootstrapper& AddSystem()
		{
			SystemInfos.AppendConstruct(S::GetSystemName(), new S());
			return *this;
		}

		template<typename S,typename D>
		Bootstrapper& AddDependency()
		{
			SystemInfos[S::GetSystemName()].Dependencies.Append(D::GetSystemName());
			return *this;
		}

	private:
		void Boot(NxFr::Array<System*>& Systems);
		void Unboot(NxFr::Array<System*>& Systems);

		void ExecuteSteps();
		void CreateSystems(NxFr::Array<System*>& Systems);
		void DestroySystems(NxFr::Array<System*>& Systems);
		void SortSystems(NxFr::Array<System*>& Systems);

		NxFr::Event<> Steps;
		NxFr::Dictionary<NxFr::StringView, SystemInfo> SystemInfos;
	};
}
