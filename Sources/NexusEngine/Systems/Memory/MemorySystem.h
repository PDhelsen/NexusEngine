#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Memory/HandleManager.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NX_ENGINE_API extern const NxFr::StringId MemoryAllocatedId;
		NX_ENGINE_API extern const NxFr::StringId MemoryAllocationId;
		NX_ENGINE_API extern const NxFr::StringId PlatformMemoryId;
	}
}

namespace NxEn
{
	class MemorySystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, MemorySystem)

		NX_ENGINE_API static NxEn::HandleManager* GetHandleManager();
		NX_ENGINE_API static NxEn::Allocator* GetAllocator(AllocatorType Type);
		NX_ENGINE_API static uint64 GetSmallAllocationSize(uint64 Size);
		NX_ENGINE_API static uint64 GetAllocatorSize(AllocatorType Type);

		NX_ENGINE_API MemorySystem();
		NX_ENGINE_API ~MemorySystem();

		NX_ENGINE_API void Defragment(bool Full = false);
		NX_ENGINE_API float GetDefragmentBudget() const;
		NX_ENGINE_API void SetDefragmentBudget(float Budget);

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		void Defragment(float Budget, bool All);
		void RecordMemoryStats();

	private:
		bool FrameFlag;
	};
}
