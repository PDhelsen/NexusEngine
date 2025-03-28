#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	enum class AllocatorType
	{
		Raw,
		General,
		Temp,
		Temp2,
		Constant,
		Small,
		Managed,
		COUNT
	};

	class MemoryManager
	{
	public:
		struct SmallAllocatorParms
		{
			uint64 Smallest;
			uint64 Largest;
		};

		static inline uint64 DefaultAllocationRequestSize = 1024;

		NEXUS_ENGINE_API static NxFr::Allocator* Allocator(AllocatorType Type, uint64 Size = DefaultAllocationRequestSize, uint64 Alignement = NxFr::Memory::DefaultAlignement);
		NEXUS_ENGINE_API static NxFr::HandleManager* Handles();

		NEXUS_ENGINE_API MemoryManager(const NxFr::Array<uint64, (uint64)AllocatorType::COUNT>& Sizes, SmallAllocatorParms SmallParams, uint64 HandlesPerManager, float DefragmentBudget);
		NEXUS_ENGINE_API ~MemoryManager();

		NEXUS_ENGINE_API void Tick();
		NEXUS_ENGINE_API void Clear();
		NEXUS_ENGINE_API void Defragment(bool Full = false);

		NEXUS_ENGINE_API NxFr::Allocator* GetAllocator(AllocatorType Type, uint64 Size = DefaultAllocationRequestSize, uint64 Alignement = NxFr::Memory::DefaultAlignement);
		NEXUS_ENGINE_API NxFr::HandleManager* GetHandlesManager();

		float GetDefragmentBudget() const { return DefragmentBudget; }
		void SetDefragmentBudget(float Budget) { DefragmentBudget = Budget; }

	private:
		NxFr::Allocator* FindOrCreateAllocator(AllocatorType Type, uint64 Size, uint64 Alignement);
		NxFr::Allocator* FindAllocator(AllocatorType Type, uint64 Size, uint64 Alignement);
		NxFr::Allocator* CreateAllocator(AllocatorType Type, uint64 Size, uint64 Stride);
		void CreateAllocatorContainers();
		void ClearAllocatorContainers(bool Force);
		void EmptyAllocators(AllocatorType Type);
		uint64 GetSmallAllocationSize(uint64 Size) const;
		uint64 GetAllocatorSize(AllocatorType Type) const;

		NxFr::HandleManager* FindOrCreateHandlesManager();
		NxFr::HandleManager* FindHandleManager();
		NxFr::HandleManager* CreateHandleManager();
		void ClearHandleManagerContainers(bool Force);

		void Defragment(float Budget, bool All);

	private:
		NxFr::Array<NxFr::List<NxFr::Allocator*>, (uint64)AllocatorType::COUNT> Allocators;
		NxFr::Array<uint64, (uint64)AllocatorType::COUNT> Sizes;
		SmallAllocatorParms SmallParams;

		NxFr::List<NxFr::HandleManager*> HandleManagers;
		uint64 HandlesPerManager;

		float DefragmentBudget;
		uint64 DefragmentAllocatorIndex;
		uint64 DefragmentHandleManagerIndex;

		bool FrameFlag;
	};
}
