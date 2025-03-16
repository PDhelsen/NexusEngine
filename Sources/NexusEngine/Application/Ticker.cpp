#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Ticker.h"

namespace NxEn
{
	Ticker::SystemInfo::SystemInfo(System* Target, TickBucket Bucket, float TickRate, bool FixedTimeStep)
		: Target(Target), Bucket(Bucket), Timer(0), TickRate(TickRate), FixedTimeStep(FixedTimeStep)
	{
	}

	Ticker::Ticker()
	{
	}

	Ticker::~Ticker()
	{

	}
	Ticker& Ticker::AddSystem(System* Target, TickBucket Bucket, float TickRate, bool FixedTimeStep)
	{
		Systems.AppendConstruct(Target, Bucket, ComputeTickRate(TickRate, FixedTimeStep), FixedTimeStep);
		Dependencies.Append(Target->GetObjectType(), SystemDependencies());
		return *this;
	}

	Ticker& Ticker::AddDependency(NxFr::StringId Target, NxFr::StringId Dependency)
	{
		Dependencies[Target].Dependencies.Append(Dependency);
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

			// Split by Bucket
			for (auto It = Systems.Begin(); It != Systems.End(); ++It)
			{
				if (It->Bucket == Bucket)
				{
					NxFr::StringId Type = It->Target->GetObjectType();

					// Validate dependencies
					SystemDependencies& RawDependencies = Dependencies[Type];
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
		}

		NEXUS_LOG(Info, Default, "Tick order:")
		for (auto& Info : Systems)
		{
			NEXUS_LOG(Info, Default, "- %s", Info.Target->GetObjectType().C());
		}
	}

	void Ticker::Tick(float DeltaTime)
	{
		for (auto& Info : Systems)
		{
			float TimeStep = ComputeTimeStep(Info, DeltaTime);
			if (TimeStep > 0.0f)
			{
				Info.Target->Tick(TimeStep);
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
