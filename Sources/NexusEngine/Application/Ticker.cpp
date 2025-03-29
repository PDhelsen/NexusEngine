#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Ticker.h"

namespace NxEn
{
	Ticker::SystemInfo::SystemInfo(System* Target, TickBucket Bucket, float TickRate, bool FixedTimeStep)
		: Target(Target), Bucket(Bucket), Timer(0), TickRate(TickRate), FixedTimeStep(FixedTimeStep)
	{
	}

	Ticker::Ticker()
		: Systems(), SystemsPerBuckets((uint64)TickBucket::COUNT), SystemsDependencies(), OnTicks((uint64)TickBucket::COUNT), OnTicksOnce((uint64)TickBucket::COUNT)
	{
		
	}

	Ticker::~Ticker()
	{

	}

	void Ticker::AppendTickCallback(const Signature& Callback, TickBucket Bucket, NxFr::StringView Tag)
	{
		OnTicks[(uint64)Bucket].AppendConstruct(Tag, Callback);
	}

	void Ticker::AppendTickOnceCallback(const Signature& Callback, TickBucket Bucket, NxFr::StringView Tag)
	{
		OnTicksOnce[(uint64)Bucket].AppendConstruct(Tag, Callback);
	}

	void Ticker::RemoveTickCallback(const Signature& Callback, TickBucket Bucket, NxFr::StringView Tag)
	{
		uint64 Index = 0;
		bool Found = false;

		auto& Callbacks = OnTicks[(uint64)Bucket];
		for (auto It = Callbacks.Begin(); It != Callbacks.End(); ++It)
		{
			if (It.Get().GetSecond() == Callback)
			{
				Found = true;
				Index = It.Id();
				break;
			}
		}

		if (Found)
		{
			Callbacks.Remove(Index);
		}
	}

	Ticker& Ticker::AppendSystem(System* Target, TickBucket Bucket, float TickRate, bool FixedTimeStep)
	{
		Systems.AppendConstruct(Target, Bucket, ComputeTickRate(TickRate, FixedTimeStep), FixedTimeStep);
		SystemsDependencies.Append(Target->GetObjectType(), SystemDependencies());
		return *this;
	}

	Ticker& Ticker::AppendDependency(NxFr::StringId Target, NxFr::StringId Dependency)
	{
		SystemsDependencies[Target].Dependencies.Append(Dependency);
		return *this;
	}

	void Ticker::SetTickRate(System* Target, float TickRate, bool FixedTimeStep)
	{
		for (auto& Info : Systems)
		{
			if (Info.Target == Target)
			{
				Info.TickRate = ComputeTickRate(TickRate, FixedTimeStep);
				break;
			}
		}
	}

	void Ticker::Run(const SystemManager& Manager)
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no systems to tick");
			return;
		}

		uint64 SortedIndex = 0;
		NxFr::Dictionary<NxFr::StringId, NxEn::SystemDependencies> DependenciesPerBucket(GetSystemsCount());
		for (uint64 BucketIndex = 0; BucketIndex < (uint64)TickBucket::COUNT; ++BucketIndex)
		{
			TickBucket Bucket = (TickBucket)BucketIndex;
			DependenciesPerBucket.Clear();

			SystemRange Range = { 0, 0 };
			Range.Start = SortedIndex;

			// Split by Bucket
			for (auto It = Systems.Begin(); It != Systems.End(); ++It)
			{
				if (It->Bucket == Bucket)
				{
					NxFr::StringId Type = It->Target->GetObjectType();

					// Validate dependencies
					SystemDependencies& RawDependencies = SystemsDependencies[Type];
					SystemDependencies CleanedDependencies;
					for (auto& D : RawDependencies.Dependencies)
					{
						for (auto& S : Systems)
						{
							if (D == S.Target->GetObjectType())
							{
								if (Bucket == S.Bucket)
								{
									CleanedDependencies.Dependencies.Append(D);
								}
								else
								{
									NEXUS_LOG(Warning, Default, "System (%s) can depend only on another system from the same bucket. %s is not in the same bucket, dependency will be ignored", Type.C(), D.C());
								}
							}
						}
					}

					DependenciesPerBucket.Append(Type, CleanedDependencies);
				}
			}

			if (DependenciesPerBucket.GetCount() == 0)
			{
				Range.End = SortedIndex;
				SystemsPerBuckets[BucketIndex] = Range;
				continue;
			}

			// Sort System in Array
			NxFr::Array<System*> SortedSystemsPerBucket = Manager.SortSystems(DependenciesPerBucket);
			for (auto It = SortedSystemsPerBucket.Begin(); It != SortedSystemsPerBucket.End(); ++It)
			{
				uint64 UnsortedIndex = 0;
				for (UnsortedIndex = 0; UnsortedIndex < Systems.GetCount(); ++UnsortedIndex)
				{
					if (It.Get() == Systems[UnsortedIndex].Target)
					{
						break;
					}
				}

				if (UnsortedIndex != SortedIndex)
				{
					Systems.Swap(UnsortedIndex, SortedIndex);
				}

				SortedIndex++;
			}

			Range.End = SortedIndex;
			SystemsPerBuckets[BucketIndex] = Range;
		}

		NEXUS_LOG(Info, Default, "Tick order:")
		for (auto& Info : Systems)
		{
			NEXUS_LOG(Info, Default, "- %s", Info.Target->GetObjectType().C());
		}
	}

	void Ticker::Tick(float DeltaTime)
	{
		for (uint64 BucketIndex = 0; BucketIndex < (uint64)TickBucket::COUNT; ++BucketIndex)
		{
			auto& OnTickOnce = OnTicksOnce[BucketIndex];
			if (OnTickOnce.GetCount() > 0)
			{
				NEXUS_PROFILE_SCOPE("Tick Once");
				for (auto& Function : OnTickOnce)
				{
					NEXUS_PROFILE_SCOPE(Function.GetFirst());

					Function.GetSecond().Invoke();
				}

				OnTickOnce.Clear();
			}

			auto& OnTick = OnTicks[BucketIndex];
			if (OnTick.GetCount() > 0)
			{
				NEXUS_PROFILE_SCOPE("Tick");

				for (auto& Function : OnTick)
				{
					NEXUS_PROFILE_SCOPE(Function.GetFirst());

					Function.GetSecond().Invoke();
				}
			}

			SystemRange Range = SystemsPerBuckets[BucketIndex];
			if (Range.Start != Range.End)
			{
				NEXUS_PROFILE_SCOPE("Systems");

				for (uint64 SystemIndex = Range.Start; SystemIndex < Range.End; SystemIndex++)
				{
					SystemInfo& Info = Systems[SystemIndex];
					float TimeStep = ComputeTimeStep(Info, DeltaTime);
					if (TimeStep > 0.0f)
					{
						NEXUS_PROFILE_SCOPE(Info.Target->GetObjectType().C());

						Info.Target->Tick(TimeStep);
					}
				}
			}
		}
	}

	float Ticker::ComputeTimeStep(SystemInfo& Info, float DeltaTime) const
	{
		Info.Timer += DeltaTime;
		
		if (Info.Timer < Info.TickRate)
		{
			return 0.0f;
		}
		
		float Timer = Info.FixedTimeStep ? Info.TickRate : Info.Timer;
		Info.Timer -= Timer;
		return Timer;
	}

	float Ticker::ComputeTickRate(float TickRate, bool FixedTimeStep) const
	{
		NEXUS_ASSERT(!FixedTimeStep || (FixedTimeStep && TickRate > 0.0f), Default, "The system has to either no require a fixed timestep or provide a tick rate greater than 0");
		return TickRate > 0.0f ? 1.0f / TickRate * (float)NxFr::Time::SecondToMilli : 0.0f;
	}
}
