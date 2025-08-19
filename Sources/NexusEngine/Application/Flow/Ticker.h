#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/SystemManager.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NEXUS_ENGINE_API extern const NxFr::StringId FpsId;
		NEXUS_ENGINE_API extern const NxFr::StringId TimerMainId;
	}
}

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
			System* Instance;
			TickBucket Bucket;
			float Timer;
			float TickRate;
			bool FixedTimeStep;

			SystemInfo(NxFr::StringId Type, TickBucket Bucket, float TickRate, bool FixedTimeStep);
			void Patch(NxFr::StringId Type);
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
		inline static const float HighFrequency = 60.0f;
		inline static const float MediumFrequency = 30.0f;
		inline static const float LowFrequency = 10.0f;
		inline static const float TwicePerSecond = 2.0f;
		inline static const float OncePerSecond = 1.0f;

		NEXUS_ENGINE_API Ticker();
		NEXUS_ENGINE_API ~Ticker();

		NEXUS_ENGINE_API void AppendTickCallback(const Signature& Callback, TickBucket Bucket = TickBucket::Project, NxFr::StringView Tag = "Tick Callback");
		NEXUS_ENGINE_API void AppendTickOnceCallback(const Signature& Callback, TickBucket Bucket = TickBucket::Project, NxFr::StringView Tag = "Tick Callback");
		NEXUS_ENGINE_API void RemoveTickCallback(const Signature& Callback, TickBucket Bucket = TickBucket::Project);

		template<typename T>
		Ticker& AppendSystem(TickBucket Bucket, float TickRate = 0.0f, bool FixedTimeStep = false) { return AppendSystem(T::GetClassType(), Bucket, TickRate, FixedTimeStep); }
		template<typename T, typename D>
		Ticker& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }

		NEXUS_ENGINE_API Ticker& AppendSystem(NxFr::StringId Type, TickBucket Bucket, float TickRate = 0.0f, bool FixedTimeStep = false);
		NEXUS_ENGINE_API Ticker& AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency);

		NEXUS_ENGINE_API void SetTickRate(NxFr::StringId Type, float TickRate, bool FixedTimeStep = false);

		uint64 GetSystemsCount() const { return Systems.GetCount(); }

	private:
		void Run();
		void Tick(float DeltaTime);

		float ComputeTimeStep(SystemInfo& Info, float DeltaTime) const;
		float ComputeTickRate(float TickRate, bool FixedTimeStep) const;
		void FlushCallbackBuffer();
		void PatchSystem(NxFr::StringId Type);

	private:
		NxFr::List<SystemInfo> Systems;
		NxFr::Array<SystemRange> SystemsPerBuckets;
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> SystemsDependencies;

		NxFr::Array<NxFr::List<NxFr::Tuple<Signature, NxFr::StringView>>> OnTicks;
		NxFr::Array<NxFr::List<NxFr::Tuple<Signature, NxFr::StringView>>> OnTicksOnce;
		NxFr::List<CallbackInfo> CallbacksBuffer;
	};
}
