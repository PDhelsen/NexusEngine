#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

namespace NxEn
{
	// TODO: Convert to SettingsSystem
	static float DefragmentBudget = 1.0f;
	static uint64 HandlesPerManager = 1024;
	static MemorySystem::SmallAllocatorParms SmallParams = { 32, 256 };
	static uint64 Sizes[(uint64)AllocatorType::COUNT] = { 0, 1024, 1024, 1024, 1024, 1024, 1024, };

	NEXUS_OBJECT_IMPLEMENTATION(MemorySystem)

	NxFr::Allocator* MemorySystem::Allocator(AllocatorType Type, uint64 Size, uint64 Alignement)
	{
		MemorySystem* Memory = Application::GetInstance()->GetSystems().GetSystem<MemorySystem>();
		return Memory->GetAllocator(Type, Size, Alignement);
	}

	NxFr::HandleManager* MemorySystem::Handles()
	{
		MemorySystem* Memory = Application::GetInstance()->GetSystems().GetSystem<MemorySystem>();
		return Memory->GetHandlesManager();
	}

	MemorySystem::MemorySystem()
		: Allocators(), HandleManagers(), DefragmentAllocatorIndex(0), DefragmentHandleManagerIndex(0), FrameFlag(false)
	{
	}

	MemorySystem::~MemorySystem()
	{
	}

	NxFr::Allocator* MemorySystem::GetAllocator(AllocatorType Type, uint64 Size, uint64 Alignement)
	{
		Size = Type == AllocatorType::Small ? GetSmallAllocationSize(Size) : Size;
		return FindOrCreateAllocator(Type, Size, Alignement);
	}

	NxFr::HandleManager* MemorySystem::GetHandlesManager()
	{
		return FindOrCreateHandlesManager();
	}

	void MemorySystem::Clear()
	{
		ClearHandleManagerContainers(false);
		ClearAllocatorContainers(false);
	}

	void MemorySystem::Defragment(bool Full)
	{
		Defragment(DefragmentBudget, Full);
	}

	float MemorySystem::GetDefragmentBudget() const
	{
		return DefragmentBudget;
	}

	void MemorySystem::SetDefragmentBudget(float Budget)
	{
		DefragmentBudget = Budget;
	}

	void MemorySystem::OnInitialize()
	{
		System::OnInitialize();

		NEXUS_ASSERT(NxFr::Math::IsPowerOfTwo(SmallParams.Smallest) && NxFr::Math::IsPowerOfTwo(SmallParams.Largest), Default, "SmallAllocatorParams have to be PowerOfTwo");
		NEXUS_ASSERT(Sizes[0] == 0, Default, "Can't set the size of the Raw allocator");

		CreateHandleManager();
		CreateAllocatorContainers();
	}

	void MemorySystem::OnShutdown()
	{
		ClearHandleManagerContainers(true);
		ClearAllocatorContainers(true);

		System::OnShutdown();
	}

	void MemorySystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		EmptyAllocators(AllocatorType::Temp);
		if (FrameFlag)
		{
			EmptyAllocators(AllocatorType::Temp2);
		}

		Defragment(false);
		RecordMemoryStats();

		FrameFlag = !FrameFlag;
	}

	NxFr::Allocator* MemorySystem::FindOrCreateAllocator(AllocatorType Type, uint64 Size, uint64 Alignement)
	{
		if (Type == AllocatorType::Raw)
		{
			return nullptr;
		}

		NxFr::Allocator* Result = FindAllocator(Type, Size, Alignement);
		if (Result == nullptr)
		{
			Result = CreateAllocator(Type, GetAllocatorSize(Type), Size);
			Allocators[(uint64)Type].Append(Result);
		}

		return Result;
	}

	NxFr::Allocator* MemorySystem::FindAllocator(AllocatorType Type, uint64 Size, uint64 Alignement)
	{
		NxFr::Allocator* Result = nullptr;

		NxFr::List<NxFr::Allocator*>& Allocs = Allocators[(uint64)Type];
		for (NxFr::Allocator* Alloc : Allocs)
		{
			if (!Alloc)
			{
				continue;
			}

			if (Type == AllocatorType::Small && static_cast<NxFr::PoolAllocator*>(Alloc)->GetStride() != Size)
			{
				continue;
			}

			if (!Alloc->CanAllocate(Size, Alignement))
			{
				continue;
			}

			Result = Alloc;
		}

		return Result;
	}

	NxFr::Allocator* MemorySystem::CreateAllocator(AllocatorType Type, uint64 Size, uint64 Stride)
	{
		NxFr::AllocatorContext Context(nullptr);
		NxFr::Allocator* Alloc = nullptr;

		switch (Type)
		{
		case NxEn::AllocatorType::General:
			Alloc = new NxFr::HeapAllocator(Size);
			break;
		case NxEn::AllocatorType::Temp:
		case NxEn::AllocatorType::Temp2:
		case NxEn::AllocatorType::Constant:
			Alloc = new NxFr::StackAllocator(Size);
			break;
		case NxEn::AllocatorType::Small:
			Alloc = new NxFr::PoolAllocator(Size, Stride);
			break;
		}

		return Alloc;
	}

	void MemorySystem::CreateAllocatorContainers()
	{
		NxFr::AllocatorContext Context(nullptr);

		for (uint64 Index = 0; Index < (uint64)AllocatorType::COUNT; ++Index)
		{
			Allocators.AssignConstruct(Index, 2, nullptr);
		}
	}

	void MemorySystem::ClearAllocatorContainers(bool Force)
	{
		NxFr::AllocatorContext Context(nullptr);
		NxFr::Array<NxFr::List<uint64>, (uint64)AllocatorType::COUNT> ToRemove;

		for (auto TypeIt = Allocators.Begin(); TypeIt != Allocators.End(); ++TypeIt)
		{
			NxFr::List<NxFr::Allocator*>& Allocs = TypeIt.Get();
			for (auto AllocatorIt = Allocs.Begin(); AllocatorIt != Allocs.End(); ++AllocatorIt)
			{
				NxFr::Allocator* Alloc = AllocatorIt.Get();
				if (Alloc && (Alloc->IsEmpty() || Force))
				{
					ToRemove[TypeIt.Id()].Append(AllocatorIt.Id());
					delete Alloc;
				}
			}
		}

		for (auto TypeIt = ToRemove.Begin(); TypeIt != ToRemove.End(); ++TypeIt)
		{
			NxFr::List<uint64>& Allocs = TypeIt.Get();
			for (auto AllocatorIt = Allocs.BeginReverse(); AllocatorIt != Allocs.EndReverse(); --AllocatorIt)
			{
				Allocators[TypeIt.Id()].Remove(Allocs[AllocatorIt.Id()]);
			}
		}
	}

	void MemorySystem::EmptyAllocators(AllocatorType Type)
	{
		NxFr::List<NxFr::Allocator*>& Allocs = Allocators[(uint64)Type];
		for (NxFr::Allocator* Alloc : Allocs)
		{
			Alloc->Clear();
		}
	}

	uint64 MemorySystem::GetSmallAllocationSize(uint64 Size) const
	{
		if (Size > SmallParams.Largest)
		{
			NEXUS_LOG(Warning, Default, "Requested small allocation size is too large. Allocation will come from the Raw allocator");
			return 0;
		}

		if (Size < SmallParams.Smallest)
		{
			NEXUS_LOG(Warning, Default, "Requested small allocation size is lower than the smallest. It will be round up to the smallest");
			return SmallParams.Smallest;
		}

		if (Size == SmallParams.Largest)
		{
			return SmallParams.Largest;
		}

		return NxFr::Math::NextPowerOfTwo(Size);
	}

	uint64 MemorySystem::GetAllocatorSize(AllocatorType Type) const
	{
		return Sizes[(uint64)Type];
	}

	NxFr::HandleManager* MemorySystem::FindOrCreateHandlesManager()
	{
		NxFr::HandleManager* Result = FindHandleManager();
		if (Result == nullptr)
		{
			Result = CreateHandleManager();
		}

		return Result;
	}

	NxFr::HandleManager* MemorySystem::FindHandleManager()
	{
		for (NxFr::HandleManager* Manager : HandleManagers)
		{
			if (Manager->GetCount() < Manager->GetCapacity())
			{
				return Manager;
			}
		}

		return nullptr;
	}

	NxFr::HandleManager* MemorySystem::CreateHandleManager()
	{
		NxFr::AllocatorContext Context(nullptr);

		NxFr::HandleManager* Manager = new NxFr::HandleManager(HandlesPerManager);
		HandleManagers.Append(Manager);
		return Manager;
	}

	void MemorySystem::ClearHandleManagerContainers(bool Force)
	{
		NxFr::AllocatorContext Context(nullptr);
		NxFr::List<uint64> ToRemove(HandleManagers.GetCount());

		for (auto HandlesIt = HandleManagers.Begin(); HandlesIt != HandleManagers.End(); ++HandlesIt)
		{
			NxFr::HandleManager* Manager = HandlesIt.Get();
			if (Manager && (Manager->IsEmpty() || Force))
			{
				ToRemove.Append(HandlesIt.Id());
				delete Manager;
			}
		}

		for (auto HandlesIt = ToRemove.BeginReverse(); HandlesIt != ToRemove.EndReverse(); --HandlesIt)
		{
			HandleManagers.Remove(HandlesIt.Id());
		}
	}

	// TODO: Defragmentation might not be full because Handle might be scattered across multiple manager
	void MemorySystem::Defragment(float Budget, bool All)
	{
		NxFr::Stopwatch Watch(true);

		DefragmentAllocatorIndex = All ? 0 : DefragmentAllocatorIndex;
		DefragmentHandleManagerIndex = All ? 0 : DefragmentHandleManagerIndex;

		NxFr::List<NxFr::Allocator*> Managed = Allocators[(uint64)AllocatorType::Managed];
		for (uint64 AllocatorIndex = 0; AllocatorIndex < Managed.GetCount(); ++AllocatorIndex)
		{
			NxFr::HeapAllocator* Heap = static_cast<NxFr::HeapAllocator*>(Managed[DefragmentAllocatorIndex]);
			DefragmentAllocatorIndex = NxFr::Math::Modulo(++DefragmentAllocatorIndex, Managed.GetCount());

			for (uint64 ManagerIndex = 0; ManagerIndex < HandleManagers.GetCount(); ++ManagerIndex)
			{
				NxFr::HandleManager* Manager = HandleManagers[DefragmentHandleManagerIndex];
				DefragmentHandleManagerIndex = NxFr::Math::Modulo(++DefragmentHandleManagerIndex, HandleManagers.GetCount());

				Heap->Defragment(Manager, All ? 0.0f : Budget);
				Budget -= (float)Watch.Peek(NxFr::Time::SecondToMilli);
			}
		}
	}

	void MemorySystem::RecordMemoryStats()
	{
		NxFr::MemoryTracker* Tracker = NxFr::MemoryTracker::GetInstance();
		NEXUS_STAT_UNSIGNEDINTEGER(StatsHeader::MemoryAllocatedId, Tracker->GetAllocatedAmount());
		NEXUS_STAT_UNSIGNEDINTEGER(StatsHeader::MemoryAllocationId, Tracker->GetAllocationCount());

		NxFr::Platform* Platform = NxFr::Platform::GetInstance();
		NxFr::Platform::MemoryInfo MemoryInfo = Platform->GetMemoryInfo();
		NEXUS_STAT_UNSIGNEDINTEGER(StatsHeader::PlatformMemoryId, MemoryInfo.CurrentUsage);
	}
}
