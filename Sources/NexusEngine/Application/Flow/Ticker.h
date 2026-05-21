#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/SystemManager.h"

namespace NxEn
{
	class NX_ENGINE_API Ticker
	{
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

		inline static const float HighFrequency = 60.0f;
		inline static const float MediumFrequency = 30.0f;
		inline static const float LowFrequency = 10.0f;
		inline static const float TwicePerSecond = 2.0f;
		inline static const float OncePerSecond = 1.0f;

		Ticker();
		~Ticker();

		void Run();
		void Tick(float DeltaTime);

		Ticker& AppendTick(TickBucket Bucket, NxFr::StringView Tag, const Signature& Callback, bool Once = false);
		Ticker& RemoveTick(TickBucket Bucket, NxFr::StringView Tag);
		template<typename T>
		Ticker& AppendSystem(TickBucket Bucket, float TickRate = 0.0f, bool FixedTimeStep = false) { return AppendSystem(T::GetClassType(), Bucket, TickRate, FixedTimeStep); }
		Ticker& AppendSystem(NxFr::StringId Type, TickBucket Bucket, float TickRate = 0.0f, bool FixedTimeStep = false);
		template<typename T, typename D>
		Ticker& AppendDependency() { return AppendDependency(T::GetClassType(), D::GetClassType()); }
		Ticker& AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency);

		void SetTickRate(NxFr::StringId Type, float TickRate, bool FixedTimeStep = false);

	private:
		struct TickInfo
		{
			TickBucket Bucket;
			NxFr::String Tag;
			Signature Callback;
			bool Once;
			bool Remove;
		};

		struct SystemInfo
		{
		public:
			TickBucket Bucket;
			NxFr::StringId Type;
			System* Instance;
			float Timer;
			float TickRate;
			bool FixedTimeStep;
		};

		inline static const NxFr::Array<NxFr::Array<NxFr::StringView>> InstrumentsMarkers =
		{
			{ "Ticks - Input", "Systems - Input" },
			{ "Ticks - Project", "Systems - Project" },
			{ "Ticks - Engine", "Systems - Engine" },
			{ "Ticks - Output", "Systems - Output" },
			{ "Ticks - Cleanup", "Systems - Cleanup" }
		};

		void FlushTicksBuffer();
		float ComputeTimeStep(SystemInfo& Info, float DeltaTime) const;
		float ComputeTickRate(float TickRate, bool FixedTimeStep) const;
		NxFr::List<TickInfo>::I GeTickInfo(NxFr::StringView Tag) const;
		NxFr::List<SystemInfo>::I GetSystemInfo(NxFr::StringId Id) const;

		NxFr::Array<NxFr::List<TickInfo>> Ticks;
		NxFr::List<TickInfo> TicksBuffer;

		NxFr::Array<NxFr::List<SystemInfo>> Systems;
		NxFr::Dictionary<NxFr::StringId, SystemDependencies> SystemsDependencies;
	};
}
