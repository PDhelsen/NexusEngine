#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

#include "NexusEngine/Core/NexusConfig.h"

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
	static NxFr::SystemAllocator& GetSystemAllocator() { static NxFr::SystemAllocator Instance; return Instance; }
	static NxFr::ContinuousAllocator& GetGeneralAllocator() { static NxFr::ContinuousAllocator Instance(NX_MEMORY_ALLOCATOR_SIZE, NxFr::ContinuousAllocator::DefaultCreator<NxFr::HeapAllocator>()); return Instance; }
	static NxFr::ContinuousAllocator& GetTempAllocator() { static NxFr::ContinuousAllocator Instance(NX_MEMORY_ALLOCATOR_SIZE, NxFr::ContinuousAllocator::DefaultCreator<NxFr::StackAllocator>()); return Instance; }
	static NxFr::ContinuousAllocator& GetConstantAllocator() { static NxFr::ContinuousAllocator Instance(NX_MEMORY_ALLOCATOR_SIZE, NxFr::ContinuousAllocator::DefaultCreator<NxFr::HeapAllocator>()); return Instance; }
	static NxFr::FixedAllocator& GetFixedAllocator() { static NxFr::FixedAllocator Instance(NX_MEMORY_ALLOCATOR_SIZE); return Instance; }
	static NxFr::ManagedAllocator& GetManagedAllocator() { static NxFr::ManagedAllocator Instance(NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_HANDLES_COUNT); return Instance; }

	static SettingSeq<uint64>* SettingAllocatorSizes = SettingSeq<uint64>::Create("Settings", "MemoryAllocatorsSize", { 0, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE });
	static SettingVar<uint64>* SettingHandlesPerManager = SettingVar<uint64>::Create("Settings", "MemoryHandlesPerManager", NX_MEMORY_HANDLES_COUNT);
	static SettingVar<float>* SettingDefragmentBudget = SettingVar<float>::Create("Settings", "MemoryDefragmentBudget", 1.0f);

	NxFr::Allocator* MemorySystem::GetAllocator(AllocatorType Type)
	{
		switch (Type)
		{
		case NxEn::AllocatorType::System: return &GetSystemAllocator();
		case NxEn::AllocatorType::General: return &GetGeneralAllocator();
		case NxEn::AllocatorType::Temp: return &GetTempAllocator();
		case NxEn::AllocatorType::Constant: return &GetConstantAllocator();
		case NxEn::AllocatorType::Fixed: return &GetFixedAllocator();
		case NxEn::AllocatorType::Managed: return &GetManagedAllocator();
		}

		return nullptr;
	}

	MemorySystem::MemorySystem()
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

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::MemoryAllocatedId, Integer, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::MemoryAllocationId, Integer, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::PlatformMemoryId, Integer, Set);

		Application::GetSystem<SettingsSystem>()->GetOnChange() += { this, & MemorySystem::ApplySettings };
	}

	void MemorySystem::OnShutdown()
	{
		Application::GetSystem<SettingsSystem>()->GetOnChange() -= { this, & MemorySystem::ApplySettings };

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

	void MemorySystem::ApplySettings()
	{
		GetGeneralAllocator().SetBucketSize(SettingAllocatorSizes->GetValue()[(uint64)AllocatorType::General]);
		GetTempAllocator().SetBucketSize(SettingAllocatorSizes->GetValue()[(uint64)AllocatorType::Temp]);
		GetConstantAllocator().SetBucketSize(SettingAllocatorSizes->GetValue()[(uint64)AllocatorType::Constant]);
		GetFixedAllocator().SetBucketSize(SettingAllocatorSizes->GetValue()[(uint64)AllocatorType::Fixed]);
		GetManagedAllocator().SetBucketSize(SettingAllocatorSizes->GetValue()[(uint64)AllocatorType::Managed]);

		GetManagedAllocator().SetHandleBucketSize(SettingHandlesPerManager->GetValue());
	}

	void MemorySystem::DefragmentManagedAllocators(float Budget)
	{
		NX_INSTUMENT_SCOPE("Defragment managed allocators");

		GetManagedAllocator().Defragment(Budget);
	}

	void MemorySystem::ClearTempAllocators()
	{
		NX_INSTUMENT_SCOPE("Clear temp allocators");

		GetTempAllocator().Clear();
	}
}
