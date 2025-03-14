#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	class Ticker
	{
		friend class Application;

	public:
		enum class Bucket
		{
			Input,
			Project,
			Engine,
			Output,
			Cleanup,

			COUNT = 5
		};

	private:
		struct SystemInfo
		{
			System* Target;
			NxFr::StringView Tag;

			Bucket TickBucket;
			float TickRate;
			float TickTimer;
			bool FixedTimeStep;

			uint64 WaitOn;
			NxFr::List<NxFr::StringId> Dependencies;
			NxFr::List<NxFr::StringId> Dependents;

			NEXUS_ENGINE_API SystemInfo(System* Target, NxFr::StringView Tag, Bucket TickBucket, float TickRate, bool FixedTimeStep);
		};

	public:
		NEXUS_ENGINE_API Ticker();
		NEXUS_ENGINE_API ~Ticker();

		template<typename S>
		Ticker& AddSystem(Bucket TickBucket, float TickRate = 0.0, bool FixedTimeStep = false)
		{
			NEXUS_ASSERT(!FixedTimeStep || (FixedTimeStep && TickRate > 0), Default, "When using a FixedTimeStep, the TickRate has to be specified and greater than 0");

			NxFr::StringId Type = S::GetClassType();
			System* System = Application::GetInstance()->GetSystem(Type);
			TickRate = (TickRate > 0.0f ? 1.0f / TickRate : 0.0f) * (float)NxFr::Time::SecondToMilli;

			SystemInfos.Append(Type, { System, Type.C(), TickBucket, TickRate, FixedTimeStep });
			return *this;
		}

		template<typename S, typename D>
		Ticker& AddDependency()
		{
			SystemInfos[S::GetClassType()].Dependencies.Append(D::GetClassType());
			return *this;
		}

		uint64 GetSystemsCount() const { return SystemInfos.GetCount(); }

	private:
		void Run();

		float ComputeTimeStep(SystemInfo& Info, float DeltaTime) const;
		NxFr::Array<SystemInfo*> SortSystems();

	private:
		NxFr::Dictionary<NxFr::StringId, SystemInfo> SystemInfos;
	};
}

