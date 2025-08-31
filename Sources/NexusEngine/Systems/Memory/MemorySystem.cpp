#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId MemoryAllocatedId = "Memory - Allocated"_Sid;
		const NxFr::StringId MemoryAllocationId = "Memory - Allocation"_Sid;
		const NxFr::StringId PlatformMemoryId = "Platform - Memory"_Sid;
	}
}

namespace NxEn
{
	// TODO: Convert to SettingsSystem
	static float DefragmentBudget = 1.0f;
	static uint64 HandlesPerManager = 1024;
	static uint64 SmallParamsSmallest = 32, SmallParamsLargest = 256;
	static uint64 Sizes[(uint64)AllocatorType::COUNT] = { 0, 1024, 1024, 1024, 1024, 1024, 1024, };

	static HandleManager& GetHandles() { static HandleManager Instance(HandlesPerManager); return Instance; }
	static Allocator& GetRawAllocator() { static Allocator Instance(AllocatorType::Raw); return Instance; }
	static Allocator& GetGeneralAllocator() { static Allocator Instance(AllocatorType::General); return Instance; }
	static Allocator& GetTempAllocator() { static Allocator Instance(AllocatorType::Temp); return Instance; }
	static Allocator& GetTemp2Allocator() { static Allocator Instance(AllocatorType::Temp2); return Instance; }
	static Allocator& GetConstantAllocator() { static Allocator Instance(AllocatorType::Constant); return Instance; }
	static Allocator& GetSmallAllocator() { static Allocator Instance(AllocatorType::Small); return Instance; }
	static Allocator& GetManagedAllocator() { static Allocator Instance(AllocatorType::Managed); return Instance; }

	NEXUS_OBJECT_IMPLEMENTATION(MemorySystem)

	NxEn::HandleManager* MemorySystem::GetHandleManager()
	{
		return &GetHandles();
	}

	NxEn::Allocator* MemorySystem::GetAllocator(AllocatorType Type)
	{
		switch (Type)
		{
		case NxEn::AllocatorType::Raw: return &GetRawAllocator();
		case NxEn::AllocatorType::General: return &GetGeneralAllocator();
		case NxEn::AllocatorType::Temp: return &GetTempAllocator();
		case NxEn::AllocatorType::Temp2: return &GetTemp2Allocator();
		case NxEn::AllocatorType::Constant: return &GetConstantAllocator();
		case NxEn::AllocatorType::Small: return &GetSmallAllocator();
		case NxEn::AllocatorType::Managed: return &GetManagedAllocator();
		}

		return nullptr;
	}

	uint64 MemorySystem::GetSmallAllocationSize(uint64 Size)
	{
		if (Size > SmallParamsLargest)
		{
			NEXUS_LOG(Warning, System, "Requested small allocation size is too large. Allocation will come from the Raw allocator");
			return 0;
		}

		if (Size < SmallParamsSmallest)
		{
			NEXUS_LOG(Warning, System, "Requested small allocation size is lower than the smallest. It will be round up to the smallest");
			return SmallParamsSmallest;
		}

		if (Size == SmallParamsLargest)
		{
			return SmallParamsLargest;
		}

		return NxFr::Math::NextPowerOfTwo(Size);
	}

	uint64 MemorySystem::GetAllocatorSize(AllocatorType Type)
	{
		return Sizes[(uint64)Type];
	}

	MemorySystem::MemorySystem()
		: FrameFlag(false)
	{
	}

	MemorySystem::~MemorySystem()
	{
	}

	void MemorySystem::Defragment(bool Full)
	{
		NEXUS_LOG(Info, System, "Memory - Defragmentation (Full: %s)", Full ? "true" : "false");
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

		NEXUS_ASSERT(NxFr::Math::IsPowerOfTwo(SmallParamsSmallest) && NxFr::Math::IsPowerOfTwo(SmallParamsLargest), System, "SmallAllocatorParams have to be PowerOfTwo");
		NEXUS_ASSERT(Sizes[0] == 0, System, "Can't set the size of the Raw allocator");

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::MemoryAllocatedId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::MemoryAllocationId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::PlatformMemoryId, UnsignedInteger, Set);
	}

	void MemorySystem::OnShutdown()
	{
		System::OnShutdown();
	}

	void MemorySystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		GetTempAllocator().Clear();
		if (FrameFlag)
		{
			GetTemp2Allocator().Clear();
		}

		Defragment(DefragmentBudget, false);
		RecordMemoryStats();

		FrameFlag = !FrameFlag;
	}

	// TODO: Defragmentation might not be full because Handle might be scattered across multiple manager
	void MemorySystem::Defragment(float Budget, bool All)
	{
		NEXUS_PROFILE_FUNCTION();

		NxFr::Stopwatch Watch(true);

		NxFr::Set<NxFr::HandleManager*>& Managers = GetHandles().Managers;
		NxFr::Set<NxFr::Allocator*>& Allocators = GetManagedAllocator().Allocators;

		for (NxFr::Allocator* Allocator : Allocators)
		{
			NxFr::HeapAllocator* Heap = static_cast<NxFr::HeapAllocator*>(Allocator);
			for (NxFr::HandleManager* Manager : Managers)
			{
				Heap->Defragment(Manager, All ? 0.0f : Budget);
				Budget -= (float)Watch.Peek(NxFr::Time::SecondToMilli);
			}
		}
	}

	void MemorySystem::RecordMemoryStats()
	{
		NxFr::MemoryTracker* Tracker = NxFr::MemoryTracker::GetInstance();
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::MemoryAllocatedId, Tracker->GetAllocatedAmount());
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::MemoryAllocationId, Tracker->GetAllocationCount());

		NxFr::Platform* Platform = NxFr::Platform::GetInstance();
		NxFr::Platform::MemoryInfo MemoryInfo = Platform->GetMemoryInfo();
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::PlatformMemoryId, MemoryInfo.CurrentUsage);
	}
}
