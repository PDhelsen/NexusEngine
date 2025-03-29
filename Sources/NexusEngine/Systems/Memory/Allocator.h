#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

namespace NxEn
{
	class Allocator : public NxFr::Allocator
	{
	public:
		NEXUS_ENGINE_API Allocator(AllocatorType Type);
		NEXUS_ENGINE_API Allocator(const Allocator& Other) = delete;
		NEXUS_ENGINE_API Allocator(Allocator&& Other) noexcept = delete;
		NEXUS_ENGINE_API virtual ~Allocator();

		NEXUS_ENGINE_API Allocator& operator=(const Allocator& Other) = delete;
		NEXUS_ENGINE_API Allocator& operator=(Allocator&& Other) noexcept = delete;

		NEXUS_ENGINE_API virtual void Clear();
		NEXUS_ENGINE_API virtual bool CanAllocate(uint64 Size, uint64 Alignement) const;
		NEXUS_ENGINE_API virtual bool BelongToAllocator(void* Pointer) const;

		NEXUS_ENGINE_API AllocatorType GetType() const { return Type; }

	protected:
		virtual void* Allocate(uint64 Size, uint64 Alignement);
		virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement);
		virtual void Free(void* Pointer);

		NxFr::Allocator* GetAllocator(void* Pointer) const;

	private:
		NxFr::Set<NxFr::Allocator*> Allocators;
		AllocatorType Type;
	};
}
