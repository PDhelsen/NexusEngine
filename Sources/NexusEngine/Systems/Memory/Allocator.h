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

	class NX_ENGINE_API Allocator : public NxFr::Allocator
	{
		friend class MemorySystem;

	public:
		NX_NOCOPY_NOMOVE(Allocator);
		Allocator(AllocatorType Type);
		virtual ~Allocator();

		virtual void Clear();
		virtual bool CanAllocate(uint64 Size, uint64 Alignement) const;
		virtual bool BelongToAllocator(void* Pointer) const;

		AllocatorType GetType() const { return Type; }

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

	private:
		NxFr::Allocator* GetAllocator(void* Pointer) const;
		NxFr::Allocator* GetAllocator(uint64 Size, uint64 Alignement);
		NxFr::Allocator* FindAllocator(uint64 Size, uint64 Alignement);
		NxFr::Allocator* CreateAllocator(uint64 Size, uint64 Stride);
		void ClearAllocators(bool Delete);
		uint64 GetAllocatorSize();

		NxFr::List<NxFr::Allocator*> Allocators;
		AllocatorType Type;
	};
}
