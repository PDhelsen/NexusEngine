#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/SystemManager.h"

namespace NxEn
{
	class Ticker
	{
		friend class Application;

	public:
		enum class TickBucket
		{
			Input,
			Project,
			Engine,
			Output,
			Cleanup,

			COUNT
		};

	private:
		struct SystemInfo
		{
		public:
			System* Target;
			TickBucket Bucket;
			float Timer;
			float TickRate;
			bool FixedTimeStep;

			SystemInfo(System* Target, TickBucket Bucket, float TickRate, bool FixedTimeStep);
		};

	public:
		NEXUS_ENGINE_API Ticker();
		NEXUS_ENGINE_API ~Ticker();

		template<typename T, typename D>
		Ticker& AddDependency() { return AddDependency(T::GetClassType(), D::GetClassType()); }

		NEXUS_ENGINE_API Ticker& AddSystem(System* Target, TickBucket Bucket, float TickRate = 0.0f, bool FixedTimeStep = false);
		NEXUS_ENGINE_API Ticker& AddDependency(NxFr::StringId Target, NxFr::StringId Dependency);

		NEXUS_ENGINE_API void SetTickRate(System* Target, float TickRate, bool FixedTimeStep = false);

		uint64 GetSystemsCount() const { return Systems.GetCount(); }

	private:
		void Run(const SystemManager& Manager);
		void Tick(float DeltaTime);

		float ComputeTimeStep(SystemInfo& Info, float DeltaTime) const;
		float ComputeTickRate(float TickRate, bool FixedTimeStep) const;

	private:
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> Dependencies;
		NxFr::List<SystemInfo> Systems;
	};
}
