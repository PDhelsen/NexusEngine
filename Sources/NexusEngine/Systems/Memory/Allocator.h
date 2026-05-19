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
		Allocator(AllocatorType Type);
		Allocator(const Allocator& Other) = delete;
		Allocator(Allocator&& Other) noexcept = delete;
		virtual ~Allocator();

		Allocator& operator=(const Allocator& Other) = delete;
		Allocator& operator=(Allocator&& Other) noexcept = delete;

		virtual void Clear();
		virtual bool CanAllocate(uint64 Size, uint64 Alignement) const;
		virtual bool BelongToAllocator(void* Pointer) const;

		AllocatorType GetType() const { return Type; }

	protected:
		virtual void* Allocate(uint64 Size, uint64 Alignement);
		virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement);
		virtual void Free(void* Pointer);

		NxFr::Allocator* GetAllocator(void* Pointer) const;
		NxFr::Allocator* GetAllocator(uint64 Size, uint64 Alignement) const;
		NxFr::Allocator* FindAllocator(uint64 Size, uint64 Alignement) const;
		NxFr::Allocator* CreateAllocator(uint64 Size, uint64 Stride) const;
		void ClearAllocators(bool Delete);

	private:
		NxFr::Set<NxFr::Allocator*> Allocators;
		AllocatorType Type;
	};
}
