#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/Memory/MemoryManager.h"

#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEn
{
	NxFr::Allocator* MemoryManager::Allocator(AllocatorType Type, uint64 Size, uint64 Alignement)
	{
		MemoryManager& Memory = NexusEngineApplication::GetInstance()->GetMemory();
		return Memory.GetAllocator(Type, Size, Alignement);
	}

	NxFr::HandleManager* MemoryManager::Handles()
	{
		MemoryManager& Memory = NexusEngineApplication::GetInstance()->GetMemory();
		return Memory.GetHandlesManager();
	}

	MemoryManager::MemoryManager(const NxFr::Array<uint64, (uint64)AllocatorType::COUNT>& Sizes, SmallAllocatorParms SmallParams, uint64 HandlesPerManager, float DefragmentBudget)
		: Allocators(), Sizes(Sizes), HandleManagers(0, nullptr), SmallParams(SmallParams), HandlesPerManager(HandlesPerManager), DefragmentBudget(DefragmentBudget), FrameFlag(false)
	{
		NEXUS_ASSERT(NxFr::Math::IsPowerOfTwo(SmallParams.Smallest) && NxFr::Math::IsPowerOfTwo(SmallParams.Largest), Default, "SmallAllocatorParams have to be PowerOfTwo");
		NEXUS_ASSERT(Sizes[0] == 0, Default, "Can't set the size of the Raw allocator");

		CreateHandleManager();
		CreateAllocatorContainers();
	}

	MemoryManager::~MemoryManager()
	{
		ClearHandleManagerContainers(true);
		ClearAllocatorContainers(true);
	}

	void MemoryManager::Tick()
	{
		EmptyAllocators(AllocatorType::Temp);
		if (FrameFlag)
		{
			EmptyAllocators(AllocatorType::Temp2);
		}

		Defragment(false);
		RecordMemoryStats();

		FrameFlag = !FrameFlag;
	}

	void MemoryManager::Clear()
	{
		ClearHandleManagerContainers(false);
		ClearAllocatorContainers(false);
	}

	void MemoryManager::Defragment(bool Full)
	{
		Defragment(DefragmentBudget, Full);
	}

	NxFr::Allocator* MemoryManager::GetAllocator(AllocatorType Type, uint64 Size, uint64 Alignement)
	{
		Size = Type == AllocatorType::Small ? GetSmallAllocationSize(Size) : Size;
		return FindOrCreateAllocator(Type, Size, Alignement);
	}

	NxFr::HandleManager* MemoryManager::GetHandlesManager()
	{
		return FindOrCreateHandlesManager();
	}

	NxFr::Allocator* MemoryManager::FindOrCreateAllocator(AllocatorType Type, uint64 Size, uint64 Alignement)
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

	NxFr::Allocator* MemoryManager::FindAllocator(AllocatorType Type, uint64 Size, uint64 Alignement)
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

	NxFr::Allocator* MemoryManager::CreateAllocator(AllocatorType Type, uint64 Size, uint64 Stride)
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

	void MemoryManager::CreateAllocatorContainers()
	{
		NxFr::AllocatorContext Context(nullptr);

		for (uint64 Index = 0; Index < (uint64)AllocatorType::COUNT; ++Index)
		{
			Allocators.AssignConstruct(Index, 2, nullptr);
		}
	}

	void MemoryManager::ClearAllocatorContainers(bool Force)
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

	void MemoryManager::EmptyAllocators(AllocatorType Type)
	{
		NxFr::List<NxFr::Allocator*>& Allocs = Allocators[(uint64)Type];
		for (NxFr::Allocator* Alloc : Allocs)
		{
			Alloc->Clear();
		}
	}

	uint64 MemoryManager::GetSmallAllocationSize(uint64 Size) const
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

	uint64 MemoryManager::GetAllocatorSize(AllocatorType Type) const
	{
		return Sizes[(uint64)Type];
	}

	NxFr::HandleManager* MemoryManager::FindOrCreateHandlesManager()
	{
		NxFr::HandleManager* Result = FindHandleManager();
		if (Result == nullptr)
		{
			Result = CreateHandleManager();
		}

		return Result;
	}

	NxFr::HandleManager* MemoryManager::FindHandleManager()
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

	NxFr::HandleManager* MemoryManager::CreateHandleManager()
	{
		NxFr::AllocatorContext Context(nullptr);

		NxFr::HandleManager* Manager = new NxFr::HandleManager(HandlesPerManager);
		HandleManagers.Append(Manager);
		return Manager;
	}

	void MemoryManager::ClearHandleManagerContainers(bool Force)
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
	void MemoryManager::Defragment(float Budget, bool All)
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

	void MemoryManager::RecordMemoryStats()
	{
		NxFr::MemoryTracker* Tracker = NxFr::MemoryTracker::GetInstance();
		NEXUS_STAT_UNSIGNEDINTEGER(StatsHeader::MemoryAllocatedId, Tracker->GetAllocatedAmount());
		NEXUS_STAT_UNSIGNEDINTEGER(StatsHeader::MemoryAllocationId, Tracker->GetAllocationCount());

		NxFr::Platform* Platform = NxFr::Platform::GetInstance();
		NxFr::Platform::MemoryInfo MemoryInfo = Platform->GetMemoryInfo();
		NEXUS_STAT_UNSIGNEDINTEGER(StatsHeader::PlatformMemoryId, MemoryInfo.CurrentUsage);
	}
}
