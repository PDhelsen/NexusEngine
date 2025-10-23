#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

#include "NexusEngine/Core/NexusConfig.h"
#include "NexusEngine/Systems/Settings/SettingTemplate.h"

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
	static SettingVar<float>* SettingDefragmentBudget = SettingVar<float>::Create("Settings", "MemoryDefragmentBudget", 1.0f);
	static SettingVar<uint64>* SettingHandlesPerManager = SettingVar<uint64>::Create("Settings", "MemoryHandlesPerManager", 1024);
	static SettingVar<uint64>* SettingSmallParamsSmallest = SettingVar<uint64>::Create("Settings", "MemorySmallParamsSmallest", 8);
	static SettingVar<uint64>* SettingSmallParamsLargest = SettingVar<uint64>::Create("Settings", "MemorySmallParamsLargest", 256);
	static SettingSeq<uint64>* SettingSizes = SettingSeq<uint64>::Create("Settings", "MemoryAllocatorsSize", { 0, NEXUS_MEMORY_ALLOCATOR_SIZE, NEXUS_MEMORY_ALLOCATOR_SIZE, NEXUS_MEMORY_ALLOCATOR_SIZE, NEXUS_MEMORY_ALLOCATOR_SIZE, NEXUS_MEMORY_ALLOCATOR_SIZE, NEXUS_MEMORY_ALLOCATOR_SIZE, });

	static HandleManager& GetHandles() { static HandleManager Instance(SettingHandlesPerManager->GetValue()); return Instance; }
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
		if (Size > SettingSmallParamsLargest->GetValue())
		{
			NEXUS_LOG(Warning, System, "Requested small allocation size is too large. Allocation will come from the Raw allocator");
			return 0;
		}

		if (Size < SettingSmallParamsSmallest->GetValue())
		{
			NEXUS_LOG(Warning, System, "Requested small allocation size is lower than the smallest. It will be round up to the smallest");
			return SettingSmallParamsSmallest->GetValue();
		}

		if (Size == SettingSmallParamsLargest->GetValue())
		{
			return SettingSmallParamsLargest->GetValue();
		}

		return (uint64)NxFr::Math::NextPowerOfTwo(Size);
	}

	uint64 MemorySystem::GetAllocatorSize(AllocatorType Type)
	{
		if (Type == NxEn::AllocatorType::Raw)
		{
			return 0;
		}

		uint64 Size = NEXUS_MEMORY_ALLOCATOR_SIZE;
		if (SettingSizes != nullptr && SettingSizes->GetValue().GetCount() > 0)
		{
			Size = SettingSizes->GetValue()[(uint64)Type];
		}

		return Size;
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
		Defragment(SettingDefragmentBudget->GetValue(), Full);
	}

	float MemorySystem::GetDefragmentBudget() const
	{
		return SettingDefragmentBudget->GetValue();
	}

	void MemorySystem::SetDefragmentBudget(float Budget)
	{
		SettingDefragmentBudget->GetValue() = Budget;
	}

	void MemorySystem::OnInitialize()
	{
		System::OnInitialize();

		NEXUS_ASSERT(NxFr::Math::IsPowerOfTwo((uint64)SettingSmallParamsSmallest->GetValue()) && NxFr::Math::IsPowerOfTwo((uint64)SettingSmallParamsLargest->GetValue()), System, "SmallAllocatorParams have to be PowerOfTwo");
		NEXUS_ASSERT(GetAllocatorSize(NxEn::AllocatorType::Raw) == 0, System, "Can't set the size of the Raw allocator");

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

		Defragment(SettingDefragmentBudget->GetValue(), false);
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
