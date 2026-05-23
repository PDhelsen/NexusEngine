#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/Ticker.h"

namespace NxEn
{
	Ticker::Ticker()
		: Ticks((uint64)TickBucket::COUNT), TicksBuffer(),
		Systems((uint64)TickBucket::COUNT), SystemsDependencies()
	{
		
	}

	Ticker::~Ticker()
	{
		
	}

	Ticker& Ticker::AppendTick(TickBucket Bucket, NxFr::StringView Tag, const Signature& Callback, bool Once)
	{
		TicksBuffer.AppendConstruct(Bucket, Tag, Callback, Once, false);
		return *this;
	}

	Ticker& Ticker::RemoveTick(TickBucket Bucket, NxFr::StringView Tag)
	{
		TicksBuffer.AppendConstruct(Bucket, Tag, nullptr, false, true);
		return *this;
	}

	Ticker& Ticker::AppendSystem(NxFr::StringId Type, TickBucket Bucket, float TickRate, bool FixedTimeStep)
	{
		Systems[(uint64)Bucket].AppendConstruct(Bucket, Type, nullptr, ComputeTickRate(TickRate, FixedTimeStep), FixedTimeStep);
		SystemsDependencies.Append(Type, NxFr::Set<NxFr::StringId>());
		return *this;
	}

	Ticker& Ticker::AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency)
	{
		SystemsDependencies[Type].TryAppend(Dependency);
		return *this;
	}

	void Ticker::SetTickRate(NxFr::StringId Type, float TickRate, bool FixedTimeStep)
	{
		auto It = GetSystemInfo(Type);
		if (It != Systems.End()->End())
		{
			It->TickRate = ComputeTickRate(TickRate, FixedTimeStep);
			NX_LOG(Info, Application, "System %s tick rate changed to %f", Type.C(), It->TickRate);
		}
	}

	void Ticker::Run()
	{
		if (SystemsDependencies.IsEmpty())
		{
			NX_LOG(Warning, Application, "There is no systems to tick");
			return;
		}

		SystemManager& Manager = Application::GetInstance()->GetSystems();

		for (uint64 BucketIndex = 0; BucketIndex < (uint64)TickBucket::COUNT; ++BucketIndex)
		{
			NxFr::List<SystemInfo>& SystemsBucket = Systems[BucketIndex];
			NxFr::Dictionary<NxFr::StringId, NxFr::Set<NxFr::StringId>> DependenciesBucket = SystemsBucket.GetCount();

			for (uint64 SystemIndex = 0; SystemIndex < SystemsBucket.GetCount(); ++SystemIndex)
			{
				SystemInfo& Info = SystemsBucket[SystemIndex];
				NxFr::Set<NxFr::StringId>& Dependencies = SystemsDependencies[Info.Type];
				NxFr::Set<NxFr::StringId>& FilteredDependencies = DependenciesBucket.Append(Info.Type, NxFr::Set<NxFr::StringId>());

				for (const auto& Dependency : Dependencies)
				{
					auto SystemIt = GetSystemInfo(Dependency);
					if (SystemIt == Systems.End()->End())
					{
						NX_LOG(Error, Application, "There is no system with Id %s", Dependency.C());
						continue;
					}
					if (SystemIt->Bucket != (Ticker::TickBucket)BucketIndex)
					{
						NX_LOG(Warning, Application, "System %s can depend only on systems inside the same bucket", Info.Type.C());
						continue;
					}

					FilteredDependencies.Append(Dependency);
				}
			}

			if (DependenciesBucket.IsEmpty())
			{
				continue;
			}

			NxFr::Array<System*> Instances = Manager.SortSystems(DependenciesBucket);
			for (uint64 InstanceIndex = 0; InstanceIndex < Instances.GetCount(); ++InstanceIndex)
			{
				System* Instance = Instances[InstanceIndex];

				uint64 SystemIndex = 0;
				for (SystemIndex = 0; SystemIndex < SystemsBucket.GetCount(); ++SystemIndex)
				{
					if (SystemsBucket[SystemIndex].Type == Instance->GetObjectType())
					{
						break;
					}
				}

				SystemInfo& Info = SystemsBucket[SystemIndex];
				Info.Instance = Instance;
				if (SystemIndex != InstanceIndex)
				{
					NxFr::ContainerUtility::Swap<SystemInfo>(SystemsBucket, SystemIndex, InstanceIndex);
				}
			}
		}

		NX_LOG(Info, Application, "Tick order:")
			for (auto& Bucket : Systems)
			{
				for (auto& Info : Bucket)
				{
					NX_LOG(Info, Application, "- %s", Info.Instance->GetObjectType().C());
				}
			}
	}

	void Ticker::Tick(float DeltaTime)
	{
		FlushTicksBuffer();

		for (uint64 BucketIndex = 0; BucketIndex < (uint64)TickBucket::COUNT; ++BucketIndex)
		{
			NxFr::List<TickInfo>& TicksBucket = Ticks[BucketIndex];
			if (!TicksBucket.IsEmpty())
			{
				NX_INSTUMENT_SCOPE(InstrumentsMarkers[BucketIndex][0]);

				uint64 TicksIndex = 0;
				while (TicksIndex < TicksBucket.GetCount())
				{
					TickInfo& Info = TicksBucket[TicksIndex];
					NX_INSTUMENT_SCOPE(Info.Tag.C());
					Info.Callback();

					if (Info.Once)
					{
						TicksBucket.Remove(TicksIndex);
					}
					else
					{
						TicksIndex++;
					}
				}
			}

			NxFr::List<SystemInfo>& SystemsBucket = Systems[BucketIndex];
			if (!SystemsBucket.IsEmpty())
			{
				NX_INSTUMENT_SCOPE(InstrumentsMarkers[BucketIndex][1]);

				for (uint64 SystemsIndex = 0; SystemsIndex < SystemsBucket.GetCount(); ++SystemsIndex)
				{
					SystemInfo& Info = SystemsBucket[SystemsIndex];
					float TimeStep = ComputeTimeStep(Info, DeltaTime);

					if (TimeStep <= 0.0f)
					{
						continue;
					}

					NX_INSTUMENT_SCOPE(Info.Instance->GetObjectType().C());
					Info.Instance->Tick(TimeStep);
				}
			}
		}
	}

	void Ticker::FlushTicksBuffer()
	{
		for (TickInfo& Info : TicksBuffer)
		{
			if (!Info.Remove)
			{
				Ticks[(uint64)Info.Bucket].AppendConstruct(Info);
				
			}
			else
			{
				auto It = GeTickInfo(Info.Tag);
				if (It != Ticks.End()->End())
				{
					Ticks[(uint64)Info.Bucket].Remove(It.Id());
				}
			}
		}

		TicksBuffer.Clear();
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
		NX_ASSERT(!FixedTimeStep || (FixedTimeStep && TickRate > 0.0f), Application, "The system has to either no require a fixed timestep or provide a tick rate greater than 0");
		return TickRate > 0.0f ? 1.0f / TickRate : TickRate;
	}

	NxFr::List<Ticker::TickInfo>::I Ticker::GeTickInfo(NxFr::StringView Tag) const
	{
		for (auto BucketIt = Ticks.Begin(); BucketIt != Ticks.End(); ++BucketIt)
		{
			for (auto TickIt = BucketIt->Begin(); TickIt != BucketIt->End(); ++TickIt)
			{
				if (TickIt->Tag == Tag)
				{
					return TickIt;
				}
			}
		}

		return Ticks.End()->End();
	}

	NxFr::List<Ticker::SystemInfo>::I Ticker::GetSystemInfo(NxFr::StringId Id) const
	{
		for (auto BucketIt = Systems.Begin(); BucketIt != Systems.End(); ++BucketIt)
		{
			for (auto SystemIt = BucketIt->Begin(); SystemIt != BucketIt->End(); ++SystemIt)
			{
				if (SystemIt->Type == Id)
				{
					return SystemIt;
				}
			}
		}

		return Systems.End()->End();
	}
}
