#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/Ticker.h"

namespace NxEn
{
	Ticker::SystemInfo::SystemInfo(NxFr::StringId Type, TickBucket Bucket, float TickRate, bool FixedTimeStep)
		: Instance(nullptr), Bucket(Bucket), Timer(0), TickRate(TickRate), FixedTimeStep(FixedTimeStep)
	{
		Patch(Type);
	}

	void Ticker::SystemInfo::Patch(NxFr::StringId Type)
	{
		Instance = Application::GetInstance()->GetSystems().GetSystem(Type);
	}

	Ticker::Ticker()
		: Systems(), SystemsPerBuckets((uint64)TickBucket::COUNT), SystemsDependencies(), OnTicks((uint64)TickBucket::COUNT), OnTicksOnce((uint64)TickBucket::COUNT), CallbacksBuffer()
	{
		
	}

	Ticker::~Ticker()
	{
		
	}

	void Ticker::AppendTickCallback(TickBucket Bucket, NxFr::StringView Tag, const Signature& Callback)
	{
		CallbacksBuffer.AppendConstruct(Bucket, Callback, Tag, false, false);
	}

	void Ticker::AppendTickOnceCallback(TickBucket Bucket, NxFr::StringView Tag, const Signature& Callback)
	{
		CallbacksBuffer.AppendConstruct(Bucket, Callback, Tag, true, false);
	}

	void Ticker::RemoveTickCallback(TickBucket Bucket, const Signature& Callback)
	{
		CallbacksBuffer.AppendConstruct(Bucket, Callback, "", true, true);
	}

	Ticker& Ticker::AppendSystem(NxFr::StringId Type, TickBucket Bucket, float TickRate, bool FixedTimeStep)
	{
		Systems.AppendConstruct(Type, Bucket, ComputeTickRate(TickRate, FixedTimeStep), FixedTimeStep);
		SystemsDependencies.Append(Type, SystemDependencies());
		return *this;
	}

	Ticker& Ticker::AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency)
	{
		SystemsDependencies[Type].Dependencies.Append(Dependency);
		return *this;
	}

	void Ticker::SetTickRate(NxFr::StringId Type, float TickRate, bool FixedTimeStep)
	{
		for (auto& Info : Systems)
		{
			if (Info.Instance->GetObjectType() == Type)
			{
				Info.TickRate = ComputeTickRate(TickRate, FixedTimeStep);
				NEXUS_LOG(Info, Application, "System %s tick rate changed to %f", Type.C(), Info.TickRate);
				break;
			}
		}
	}

	void Ticker::Run()
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Application, "There is no systems to tick");
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
					NxFr::StringId Type = It->Instance->GetObjectType();

					// Validate dependencies
					SystemDependencies& RawDependencies = SystemsDependencies[Type];
					SystemDependencies CleanedDependencies;
					for (auto& D : RawDependencies.Dependencies)
					{
						for (auto& S : Systems)
						{
							if (D == S.Instance->GetObjectType())
							{
								if (Bucket == S.Bucket)
								{
									CleanedDependencies.Dependencies.Append(D);
								}
								else
								{
									NEXUS_LOG(Warning, Application, "System (%s) can depend only on another system from the same bucket. %s is not in the same bucket, dependency will be ignored", Type.C(), D.C());
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
			SystemManager& Manager = Application::GetInstance()->GetSystems();
			NxFr::Array<System*> SortedSystemsPerBucket = Manager.SortSystems(DependenciesPerBucket);
			for (auto It = SortedSystemsPerBucket.Begin(); It != SortedSystemsPerBucket.End(); ++It)
			{
				uint64 UnsortedIndex = 0;
				for (UnsortedIndex = 0; UnsortedIndex < Systems.GetCount(); ++UnsortedIndex)
				{
					if (It.Get()->GetObjectType() == Systems[UnsortedIndex].Instance->GetObjectType())
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

		NEXUS_LOG(Info, Application, "Tick order:")
		for (auto& Info : Systems)
		{
			NEXUS_LOG(Info, Application, "- %s", Info.Instance->GetObjectType().C());
		}
	}

	void Ticker::Tick(float DeltaTime)
	{
		FlushCallbackBuffer();

		for (uint64 BucketIndex = 0; BucketIndex < (uint64)TickBucket::COUNT; ++BucketIndex)
		{
			auto& OnTickOnce = OnTicksOnce[BucketIndex];
			if (OnTickOnce.GetCount() > 0)
			{
				NEXUS_PROFILE_SCOPE("Tick Once");
				for (uint64 Index = 0; Index < OnTickOnce.GetCount(); ++Index)
				{
					NEXUS_PROFILE_SCOPE(OnTickOnce[Index].GetSecond());

					OnTickOnce[Index].GetFirst().Invoke();
				}

				OnTickOnce.Clear();
			}

			auto& OnTick = OnTicks[BucketIndex];
			if (OnTick.GetCount() > 0)
			{
				NEXUS_PROFILE_SCOPE("Tick");

				for (uint64 Index = 0; Index < OnTick.GetCount(); ++Index)
				{
					NEXUS_PROFILE_SCOPE(OnTick[Index].GetSecond());

					OnTick[Index].GetFirst().Invoke();
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
						NEXUS_PROFILE_SCOPE(Info.Instance->GetObjectType().C());

						Info.Instance->Tick(TimeStep);
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
		NEXUS_ASSERT(!FixedTimeStep || (FixedTimeStep && TickRate > 0.0f), Application, "The system has to either no require a fixed timestep or provide a tick rate greater than 0");
		return TickRate > 0.0f ? 1.0f / TickRate : 0.0f;
	}

	void Ticker::FlushCallbackBuffer()
	{
		for (uint64 BucketIndex = 0; BucketIndex < CallbacksBuffer.GetCount(); ++BucketIndex)
		{
			CallbackInfo& Info = CallbacksBuffer[BucketIndex];
			if (Info.Remove)
			{
				uint64 Index = 0;
				bool Found = false;

				auto& Callbacks = OnTicks[(uint64)Info.Bucket];
				for (uint64 CallbackIndex = 0; CallbackIndex < Callbacks.GetCount(); ++CallbackIndex)
				{
					if (Callbacks[CallbackIndex].GetFirst() == Info.Callback)
					{
						Found = true;
						Index = CallbackIndex;
						break;
					}
				}

				if (Found)
				{
					Callbacks.Remove(Index);
				}
			}
			else
			{
				if (Info.Once)
				{
					OnTicksOnce[(uint64)Info.Bucket].AppendConstruct(Info.Callback, Info.Tag);
				}
				else
				{
					OnTicks[(uint64)Info.Bucket].AppendConstruct(Info.Callback, Info.Tag);
				}
			}
		}

		CallbacksBuffer.Clear();
	}

	void Ticker::PatchSystem(NxFr::StringId Type)
	{
		for (auto& Info : Systems)
		{
			if (Info.Instance->GetObjectType() == Type)
			{
				Info.Patch(Type);
				NEXUS_LOG(Info, Application, "System %s patch", Type.C());
				return;
			}
		}
	}
}
