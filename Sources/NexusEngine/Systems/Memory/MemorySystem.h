#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Memory/HandleManager.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NEXUS_ENGINE_API extern const NxFr::StringId MemoryAllocatedId;
		NEXUS_ENGINE_API extern const NxFr::StringId MemoryAllocationId;
		NEXUS_ENGINE_API extern const NxFr::StringId PlatformMemoryId;
	}
}

namespace NxEn
{
	class MemorySystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, MemorySystem)

		NEXUS_ENGINE_API static NxEn::HandleManager* GetHandleManager();
		NEXUS_ENGINE_API static NxEn::Allocator* GetAllocator(AllocatorType Type);
		NEXUS_ENGINE_API static uint64 GetSmallAllocationSize(uint64 Size);
		NEXUS_ENGINE_API static uint64 GetAllocatorSize(AllocatorType Type);

		NEXUS_ENGINE_API MemorySystem();
		NEXUS_ENGINE_API ~MemorySystem();

		NEXUS_ENGINE_API void Defragment(bool Full = false);
		NEXUS_ENGINE_API float GetDefragmentBudget() const;
		NEXUS_ENGINE_API void SetDefragmentBudget(float Budget);

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		void Defragment(float Budget, bool All);
		void RecordMemoryStats();

	private:
		bool FrameFlag;
	};
}
