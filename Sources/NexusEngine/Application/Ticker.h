#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/SystemManager.h"

namespace NxEn
{
	class Ticker
	{
		friend class Application;

	public:
		using Signature = NxFr::Delegate<void()>;

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

		struct SystemRange
		{
		public:
			uint64 Start = 0;
			uint64 End = 0;
		};

		struct CallbackInfo
		{
			TickBucket Bucket;
			Signature Callback;
			NxFr::StringView Tag;
			bool Once;
			bool Remove;
		};

	public:
		NEXUS_ENGINE_API Ticker();
		NEXUS_ENGINE_API ~Ticker();

		NEXUS_ENGINE_API void AppendTickCallback(const Signature& Callback, TickBucket Bucket = TickBucket::Project, NxFr::StringView Tag = "");
		NEXUS_ENGINE_API void AppendTickOnceCallback(const Signature& Callback, TickBucket Bucket = TickBucket::Project, NxFr::StringView Tag = "");
		NEXUS_ENGINE_API void RemoveTickCallback(const Signature& Callback, TickBucket Bucket = TickBucket::Project, NxFr::StringView Tag = "");

		template<typename T, typename D>
		Ticker& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }
		NEXUS_ENGINE_API Ticker& AppendSystem(System* Target, TickBucket Bucket, float TickRate = 0.0f, bool FixedTimeStep = false);
		NEXUS_ENGINE_API Ticker& AppendDependency(NxFr::StringId Target, NxFr::StringId Dependency);

		NEXUS_ENGINE_API void SetTickRate(System* Target, float TickRate, bool FixedTimeStep = false);

		uint64 GetSystemsCount() const { return Systems.GetCount(); }

	private:
		void Run(const SystemManager& Manager);
		void Tick(float DeltaTime);

		float ComputeTimeStep(SystemInfo& Info, float DeltaTime) const;
		float ComputeTickRate(float TickRate, bool FixedTimeStep) const;
		void FlushCallbackBuffer();

	private:
		NxFr::List<SystemInfo> Systems;
		NxFr::Array<SystemRange> SystemsPerBuckets;
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> SystemsDependencies;

		NxFr::Array<NxFr::List<NxFr::Tuple<Signature, NxFr::StringView>>> OnTicks;
		NxFr::Array<NxFr::List<NxFr::Tuple<Signature, NxFr::StringView>>> OnTicksOnce;
		NxFr::List<CallbackInfo> CallbacksBuffer;
	};
}
