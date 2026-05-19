#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Memory/HandleManager.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

namespace NxFr
{
	namespace StatsHeader
	{
		extern const NxFr::StringId MemoryAllocatedId;
		extern const NxFr::StringId MemoryAllocationId;
		extern const NxFr::StringId PlatformMemoryId;
	}
}

namespace NxEn
{
	class NX_ENGINE_API MemorySystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(MemorySystem)

		static NxEn::HandleManager* GetHandleManager();
		static NxEn::Allocator* GetAllocator(AllocatorType Type);
		static uint64 GetSmallAllocationSize(uint64 Size);
		static uint64 GetAllocatorSize(AllocatorType Type);

		MemorySystem();
		~MemorySystem();

		void Defragment(bool Full = false);
		float GetDefragmentBudget() const;
		void SetDefragmentBudget(float Budget);

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void Defragment(float Budget, bool All);
		void RecordMemoryStats();

	private:
		bool FrameFlag;
	};
}
