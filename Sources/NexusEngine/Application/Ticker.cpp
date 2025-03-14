#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Ticker.h"

#include "NexusEngine/Misc/DebugManager.h"

namespace NxEn
{
	Ticker::SystemInfo::SystemInfo(System* Target, NxFr::StringView Tag, Bucket TickBucket, float TickRate, bool FixedTimeStep)
		: Target(Target), Tag(Tag), TickBucket(TickBucket), TickRate(TickRate), TickTimer(0), FixedTimeStep(FixedTimeStep), WaitOn(0), Dependencies(), Dependents()
	{
	}

	Ticker::Ticker()
	{
	}

	Ticker::~Ticker()
	{
	}

	void Ticker::Run()
	{
		Application* Instance = Application::GetInstance();
		NxFr::Array<SystemInfo*> Systems = SortSystems();

		NEXUS_LOG(Info, Default, "Tick order:")
		for (auto Info : Systems)
		{
			NEXUS_LOG(Info, Default, "- %s", Info->Tag.C());
		}

		NxFr::Stopwatch Stopwatch;
		float DeltaTime = 0.0;

		while (Instance->IsRunning())
		{
			Stopwatch.Start();

			for (auto Info : Systems)
			{
				float TimeStep = ComputeTimeStep(*Info, DeltaTime);
				if (TimeStep > 0.0f)
				{
					Info->Target->Tick(TimeStep);
				}
			}

			DeltaTime = (float)Stopwatch.Stop(NxFr::Time::SecondToMilli);

			DebugManager::GetInstance()->Flush();
		}
	}

	float Ticker::ComputeTimeStep(SystemInfo& Info, float DeltaTime) const
	{
		Info.TickTimer += DeltaTime;

		if (Info.TickTimer < Info.TickRate)
		{
			return 0.0f;
		}

		float TickTimer = Info.FixedTimeStep ? Info.TickRate : Info.TickTimer;
		Info.TickTimer -= Info.FixedTimeStep ? Info.TickRate : Info.TickTimer;
		return TickTimer;
	}

	NxFr::Array<Ticker::SystemInfo*> Ticker::SortSystems()
	{
		NxFr::Array<SystemInfo*> Infos = NxFr::Array<SystemInfo*>(GetSystemsCount());
		NxFr::Queue<NxFr::StringId> Queue;
		int Index = 0;

		for (auto& [Type, Info] : SystemInfos)
		{
			for (auto& Dependency : Info.Dependencies)
			{
				if (Info.TickBucket != SystemInfos[Dependency].TickBucket)
				{
					NEXUS_LOG(Warning, Default, "A System can only depend on another one from the same bucket. Dependency of %s on %s will be ignored", Type.C(), Dependency.C());
					continue;
				}

				Info.WaitOn++;
				SystemInfos[Dependency].Dependents.Append(Type);
			}
		}

		for (uint64 TickBucketIndex = 0; TickBucketIndex < (uint64)Bucket::COUNT; ++TickBucketIndex)
		{
			Queue.Clear();
			bool BucketIsEmpty = true;
			Bucket TickBucket = (Bucket)TickBucketIndex;

			for (auto& [Type, Info] : SystemInfos)
			{
				if (Info.TickBucket != TickBucket)
				{
					continue;
				}

				BucketIsEmpty = false;

				if (Info.WaitOn != 0)
				{
					continue;
				}

				Queue.Append(Type);
			}

			NEXUS_ASSERT(BucketIsEmpty || !Queue.IsEmpty(), Default, "Impossible to start sorting the systems because each one depened on another one, probably circular dependency.");

			while (!Queue.IsEmpty())
			{
				NxFr::StringId Type = Queue.Get();
				Queue.Remove();

				SystemInfo& Info = SystemInfos[Type];
				Infos[Index++] = &Info;

				for (auto& Dependent : Info.Dependents)
				{
					if (--SystemInfos[Dependent].WaitOn == 0)
					{
						if (SystemInfos[Dependent].TickBucket == TickBucket)
						{
							Queue.Append(Dependent);
						}
					}
				}

				Info.Dependencies.Clear();
				Info.Dependents.Clear();
			}
		}

		NEXUS_ASSERT(Index == GetSystemsCount(), Default, "Some Systems were not sorted, probaly unable to resolve all the dependencies");

		return Infos;
	}
}
