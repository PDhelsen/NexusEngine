#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

namespace NxEn
{
	Allocator::Allocator(AllocatorType Type, bool Exclusive)
		: NxFr::Allocator(0), Allocators(), Type(Type), Exclusive(Exclusive)
	{
	}

	Allocator::~Allocator()
	{
		Clear();
	}

	void Allocator::Clear()
	{
		if (Exclusive)
		{
			for (NxFr::Allocator* Alloc : Allocators)
			{
				Alloc->Clear();
			}
		}
	}

	bool Allocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		// It will always be able to allocate since it request memory from MemoryManager
		return true;
	}

	bool Allocator::BelongToAllocator(void* Pointer) const
	{
		return GetAllocator(Pointer) != nullptr;
	}

	void* Allocator::Allocate(uint64 Size, uint64 Alignement)
	{
		NxFr::Allocator* Alloc = GetAllocator(Size, Alignement);

		uint64 Marker = Alloc->UsedAmount();

		void* Pointer = NxFr::Memory::Allocate(Size, Alloc, Alignement);

		uint64 Delta = Alloc->UsedAmount() - Marker;
		IncreaseAmount(Delta);

		Allocators.Append(Alloc);
		return Pointer;
	}

	void* Allocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (!Pointer)
		{
			return nullptr;
		}

		NxFr::Allocator* Alloc = GetAllocator(Pointer);
		NEXUS_ASSERT(Alloc, System, "Memory was not allocated from this allocator");

		if (Alloc->CanAllocate(Size, Alignement))
		{
			uint64 Marker = Alloc->UsedAmount();

			Pointer = NxFr::Memory::Reallocate(Pointer, Size, Alloc, Alignement);

			if (Alloc->UsedAmount() > Marker)
			{
				uint64 Delta = Alloc->UsedAmount() - Marker;
				IncreaseAmount(Delta);
			}
			else
			{
				uint64 Delta = Marker - Alloc->UsedAmount();
				DecreaseAmount(Delta);
			}
		}
		else
		{
			Free(Pointer);
			Pointer = Allocate(Size, Alignement);
		}

		return Pointer;
	}

	void Allocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		NxFr::Allocator* Alloc = GetAllocator(Pointer);
		NEXUS_ASSERT(Alloc, System, "Memory was not allocated from this allocator");

		uint64 Marker = Alloc->UsedAmount();

		NxFr::Memory::Free(Pointer, Alloc);

		uint64 Delta = Marker - Alloc->UsedAmount();
		DecreaseAmount(Delta);
	}

	NxFr::Allocator* Allocator::GetAllocator(uint64 Size, uint64 Alignement) const
	{
		MemorySystem* System = Application::GetSystem<MemorySystem>();
		if (!Exclusive)
		{
			return System->GetAllocator(Type, Size, Alignement);
		}

		Size = System->GetSmallAllocationSize(Size);
		for (NxFr::Allocator* Alloc : Allocators)
		{
			if (Alloc->CanAllocate(Size, Alignement))
			{
				return Alloc;
			}
		}

		uint64 AllocatorSize = System->GetAllocatorSize(Type);
		return System->CreateAllocator(Type, AllocatorSize, Size);
	}

	NxFr::Allocator* Allocator::GetAllocator(void* Pointer) const
	{
		for (NxFr::Allocator* Alloc : Allocators)
		{
			if (Alloc->BelongToAllocator(Pointer))
			{
				return Alloc;
			}
		}

		return nullptr;
	}
}
