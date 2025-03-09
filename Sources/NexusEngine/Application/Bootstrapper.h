#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	class Bootstrapper
	{
		friend class Application;

	public:
		using Signature = NxFr::Delegate<void()>;

	private:
		struct StepInfo
		{
		public:
			Signature Target;
			NxFr::StringView Tag;

			NEXUS_ENGINE_API StepInfo(const Signature& Target, NxFr::StringView Tag = "");
		};

		struct SystemInfo
		{
		public:
			System* Target;
			NxFr::StringView Tag;
			uint64 Remaining;
			NxFr::List<NxFr::StringId> Dependencies;
			NxFr::List<NxFr::StringId> Dependents;

			NEXUS_ENGINE_API SystemInfo(System* Target, NxFr::StringView Tag = "");
		};

	public:
		NEXUS_ENGINE_API Bootstrapper();
		NEXUS_ENGINE_API ~Bootstrapper();

		Bootstrapper& AddStep(Signature Step, NxFr::StringView Tag = "")
		{
			StepInfos.Append({ Step, Tag });
			return *this;
		}

		template<typename S>
		Bootstrapper& AddSystem()
		{
			NxFr::StringId Type = S::GetClassType();
			SystemInfos.Append(Type, { new S(), Type.C() });
			return *this;
		}

		template<typename S,typename D>
		Bootstrapper& AddDependency()
		{
			SystemInfos[S::GetClassType()].Dependencies.Append(D::GetClassType());
			return *this;
		}

		uint64 GetStepsCount() const { return StepInfos.GetCount(); }
		uint64 GetSystemsCount() const { return SystemInfos.GetCount(); }

	private:
		void RunBoot();
		void RunUnboot();

		void ExecuteSteps();
		void CreateSystems();
		void DestroySystems();
		NxFr::Array<SystemInfo*> SortSystems();

	private:
		NxFr::List<StepInfo> StepInfos;
		NxFr::Dictionary<NxFr::StringId, SystemInfo> SystemInfos;
	};
}
