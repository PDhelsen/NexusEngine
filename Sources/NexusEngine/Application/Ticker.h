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
			bool FixedTimeStep;
			uint64 WaitOn;
			NxFr::List<NxFr::StringId> Dependencies;
			NxFr::List<NxFr::StringId> Dependents;

			NEXUS_ENGINE_API SystemInfo(System* Target, NxFr::StringView Tag, Bucket TickBucket, float TickRate, bool FixedTimeStep);
		};

		struct SystemData
		{
			System* Target;
			float Rate;
			double Timer;
			bool FixedTimeStep;
		};

	public:
		NEXUS_ENGINE_API Ticker();
		NEXUS_ENGINE_API ~Ticker();

		template<typename S>
		Ticker& AddSystem(Bucket TickBucket, float TickRate = 0.0, bool FixedTimeStep = false)
		{
			NxFr::StringId Type = S::GetClassType();
			AddSystem(Type, TickBucket, TickRate, FixedTimeStep);
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
		float ComputeTimeStep(SystemData& Data, double DeltaTime) const;
		NxFr::Array<SystemData> Sort();

		NEXUS_ENGINE_API void AddSystem(NxFr::StringId Type, Bucket TickBucket, float TickRate, bool FixedTimeStep);


	private:
		NxFr::Dictionary<NxFr::StringId, SystemInfo> SystemInfos;
	};
}

