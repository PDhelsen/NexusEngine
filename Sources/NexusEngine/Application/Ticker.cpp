#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Ticker.h"

namespace NxEn
{
	Ticker::SystemInfo::SystemInfo(System* Target, NxFr::StringView Tag, Bucket TickBucket, float TickRate, bool FixedTimeStep)
		: Target(Target), Tag(Tag), TickBucket(TickBucket), TickRate(TickRate), FixedTimeStep(FixedTimeStep), WaitOn(0), Dependencies(), Dependents()
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
		NxFr::Array<SystemData> Systems = Sort();

		NEXUS_LOG(Info, Default, "Tick order:")
		for (auto& Data : Systems)
		{
			NEXUS_LOG(Info, Default, "- %s", SystemInfos[Data.Target->GetObjectType()].Tag.C());
		}

		NxFr::Stopwatch Stopwatch;
		double DeltaTime = 0.0;

		while (Instance->IsRunning())
		{
			Stopwatch.Start();

			for (auto& Data : Systems)
			{
				float TimeStep = ComputeTimeStep(Data, DeltaTime);
				if (TimeStep > 0.0f)
				{
					Data.Target->Tick(TimeStep);
				}
			}

			DeltaTime = Stopwatch.Stop(NxFr::Time::SecondToMilli);
		}
	}

	float Ticker::ComputeTimeStep(SystemData& Data, double DeltaTime) const
	{
		Data.Timer += DeltaTime;

		if (Data.Timer < Data.Rate)
		{
			return 0.0f;
		}

		float Timer = Data.FixedTimeStep ? Data.Rate : Data.Timer;
		Data.Timer -= Data.FixedTimeStep ? Data.Rate : Data.Timer;
		return Timer;
	}

	NxFr::Array<Ticker::SystemData> Ticker::Sort()
	{
		NxFr::Array<SystemData> Systems = NxFr::Array<SystemData>(GetSystemsCount());
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

		NxFr::Queue<NxFr::StringId> Queue;
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
				Systems[Index++] = SystemData { .Target = Info.Target, .Rate = Info.TickRate, .Timer = 0, .FixedTimeStep = Info.FixedTimeStep };

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

		return Systems;
	}

	void Ticker::AddSystem(NxFr::StringId Type, Bucket Bucket, float TickRate, bool FixedTimeStep)
	{
		System* System = Application::GetInstance()->GetSystem(Type);
		SystemInfos.Append(Type, { System, System->GetObjectType().C(), Bucket, TickRate, FixedTimeStep });
	}
}
