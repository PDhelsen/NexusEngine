#pragma once

#include "NexusEngine/Application/Systems/System.h"
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
		friend class Allocator;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, MemorySystem)

		struct SmallAllocatorParms
		{
			uint64 Smallest;
			uint64 Largest;
		};

		static inline uint64 DefaultAllocationRequestSize = 1024;

		NEXUS_ENGINE_API MemorySystem();
		NEXUS_ENGINE_API ~MemorySystem();

		NEXUS_ENGINE_API NxFr::Allocator* GetAllocator(AllocatorType Type, uint64 Size = DefaultAllocationRequestSize, uint64 Alignement = NxFr::Memory::DefaultAlignement);
		NEXUS_ENGINE_API NxFr::HandleManager* GetHandlesManager();
		NEXUS_ENGINE_API NxEn::Allocator* GetDefault();

		NEXUS_ENGINE_API void Clear();
		NEXUS_ENGINE_API void Defragment(bool Full = false);

		NEXUS_ENGINE_API float GetDefragmentBudget() const;
		NEXUS_ENGINE_API void SetDefragmentBudget(float Budget);

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

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
		void RecordMemoryStats();

	private:
		NxFr::Array<NxFr::List<NxFr::Allocator*>, (uint64)AllocatorType::COUNT> Allocators;
		NxFr::List<NxFr::HandleManager*> HandleManagers;
		Allocator* Default;

		uint64 DefragmentAllocatorIndex;
		uint64 DefragmentHandleManagerIndex;

		bool FrameFlag;
	};
}
