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
	static HandleManager& GetHandles() { static HandleManager Instance; return Instance; }
	static Allocator& GetRawAllocator() { static Allocator Instance(AllocatorType::Raw); return Instance; }
	static Allocator& GetGeneralAllocator() { static Allocator Instance(AllocatorType::General); return Instance; }
	static Allocator& GetTempAllocator() { static Allocator Instance(AllocatorType::Temp); return Instance; }
	static Allocator& GetTemp2Allocator() { static Allocator Instance(AllocatorType::Temp2); return Instance; }
	static Allocator& GetConstantAllocator() { static Allocator Instance(AllocatorType::Constant); return Instance; }
	static Allocator& GetSmallAllocator() { static Allocator Instance(AllocatorType::Small); return Instance; }
	static Allocator& GetManagedAllocator() { static Allocator Instance(AllocatorType::Managed); return Instance; }

	static SettingVar<float>* SettingDefragmentBudget = SettingVar<float>::Create("Settings", "MemoryDefragmentBudget", 1.0f);

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

	MemorySystem::MemorySystem()
		: Time(nullptr), Defragmentation()
	{
	}

	MemorySystem::~MemorySystem()
	{
	}

	void MemorySystem::Defragment(float Budget)
	{
		NX_INSTUMENT_FUNCTION();

		NX_LOG(Info, System, "Memory - Defragmentation (Budget: %f)", Budget);
		DefragmentManagedAllocators(Budget);
	}

	void MemorySystem::OnInitialize()
	{
		System::OnInitialize();
		Time = &Application::GetInstance()->GetTime();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::MemoryAllocatedId, Integer, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::MemoryAllocationId, Integer, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::PlatformMemoryId, Integer, Set);
	}

	void MemorySystem::OnShutdown()
	{
		Time = nullptr;
		System::OnShutdown();
	}

	void MemorySystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		DefragmentManagedAllocators(SettingDefragmentBudget->GetValue());
		ClearTempAllocators();

		NX_STAT_INTEGER(NxFr::StatsHeader::MemoryAllocatedId, NxFr::Globals::Debug::Memory->GetAllocatedAmount());
		NX_STAT_INTEGER(NxFr::StatsHeader::MemoryAllocationId, NxFr::Globals::Debug::Memory->GetAllocationCount());
		NX_STAT_INTEGER(NxFr::StatsHeader::PlatformMemoryId, NxFr::Globals::PlatformTarget->GetMemoryInfo().CurrentUsage);
	}

	// TODO: Defragmentation might not be full because Handle might be scattered across multiple manager
	void MemorySystem::DefragmentManagedAllocators(float Budget)
	{
		NX_INSTUMENT_SCOPE("Defragment");

		NxFr::Stopwatch Watch(true);
		NxFr::List<NxFr::HandleManager*>& Managers = GetHandles().Managers;
		NxFr::List<NxFr::Allocator*>& Allocators = GetManagedAllocator().Allocators;

		for (uint64 AllocatorIndex = 0; AllocatorIndex < Allocators.GetCount(); ++AllocatorIndex)
		{
			NxFr::Allocator* Allocator = Allocators[(AllocatorIndex + Defragmentation.x) % Allocators.GetCount()];
			NxFr::HeapAllocator* Heap = static_cast<NxFr::HeapAllocator*>(Allocator);

			for (uint64 ManagerIndex = 0; ManagerIndex < Managers.GetCount(); ++ManagerIndex)
			{
				NxFr::HandleManager* Manager = Managers[(ManagerIndex + Defragmentation.y) % Managers.GetCount()];
				Heap->Defragment(Manager, Budget);

				if (Budget > 0.0f)
				{
					Budget -= (float)Watch.Peek(NxFr::Time::SecondToMilli);
					if (Budget <= 0.0f)
					{
						Defragmentation.x = (AllocatorIndex + Defragmentation.x) % Allocators.GetCount();
						Defragmentation.y = (ManagerIndex + Defragmentation.y) % Managers.GetCount();
						break;
					}
				}
			}
		}
	}

	void MemorySystem::ClearTempAllocators()
	{
		NX_INSTUMENT_SCOPE("Clear Temp allocators");

		GetTempAllocator().Clear();
		if (Time->GetFrameIndex() & 1)
		{
			GetTemp2Allocator().Clear();
		}
	}
}
