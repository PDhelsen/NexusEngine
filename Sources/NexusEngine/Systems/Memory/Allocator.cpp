#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

namespace NxEn
{
	Allocator::Allocator(AllocatorType Type)
		: NxFr::Allocator(0), Allocators(11, nullptr), Type(Type)
	{
	}

	Allocator::~Allocator()
	{
		ClearAllocators(true);
	}

	void Allocator::Clear()
	{
		ClearAllocators(false);
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
		NX_ASSERT(Size < MemorySystem::GetAllocatorSize(Type) || Type == AllocatorType::Raw, Default, "Allocation size requested overflow allocator size");

		NxFr::Allocator* Alloc = GetAllocator(Size, Alignement);

		uint64 Marker = Alloc->UsedAmount();

		void* Pointer = NxFr::Memory::Allocate(Size, Alloc, Alignement);

		uint64 Delta = Alloc->UsedAmount() - Marker;
		IncreaseAmount(Delta);

		Allocators.TryAppend(Alloc);
		return Pointer;
	}

	void* Allocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (!Pointer)
		{
			return nullptr;
		}

		NX_ASSERT(Size < MemorySystem::GetAllocatorSize(Type), Default, "Allocation size requested overflow allocator size");

		NxFr::Allocator* Alloc = GetAllocator(Pointer);
		NX_ASSERT(Alloc, System, "Memory was not allocated from this allocator");

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
		NX_ASSERT(Alloc, System, "Memory was not allocated from this allocator");

		uint64 Marker = Alloc->UsedAmount();

		NxFr::Memory::Free(Pointer, Alloc);

		uint64 Delta = Marker - Alloc->UsedAmount();
		DecreaseAmount(Delta);
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

	NxFr::Allocator* Allocator::GetAllocator(uint64 Size, uint64 Alignement) const
	{
		Size = Type == AllocatorType::Small ? MemorySystem::GetSmallAllocationSize(Size) : Size;

		NxFr::Allocator* Result = FindAllocator(Size, Alignement);
		if (Result == nullptr)
		{
			Result = CreateAllocator(MemorySystem::GetAllocatorSize(Type), Size);
		}

		return Result;
	}

	NxFr::Allocator* Allocator::FindAllocator(uint64 Size, uint64 Alignement) const
	{
		for (NxFr::Allocator* Alloc : Allocators)
		{
			if (Alloc->CanAllocate(Size, Alignement))
			{
				return Alloc;
			}
		}

		return nullptr;
	}

	NxFr::Allocator* Allocator::CreateAllocator(uint64 Size, uint64 Stride) const
	{
		NxFr::Allocator::Scope Context(nullptr);
		NxFr::Allocator* Alloc = nullptr;

		switch (Type)
		{
		case AllocatorType::Raw:
			Alloc = new NxFr::RawAllocator();
			break;
		case AllocatorType::General:
		case AllocatorType::Managed:
			Alloc = new NxFr::HeapAllocator(Size);
			break;
		case AllocatorType::Temp:
		case AllocatorType::Temp2:
		case AllocatorType::Constant:
			Alloc = new NxFr::StackAllocator(Size);
			break;
		case AllocatorType::Small:
			Alloc = new NxFr::PoolAllocator(Size, Stride);
			break;
		}

		return Alloc;
	}

	void Allocator::ClearAllocators(bool Delete)
	{
		NxFr::Allocator::Scope Context(nullptr);
		NxFr::List<NxFr::Allocator*> ToDelete(Allocators.GetCount());

		for (NxFr::Allocator* Alloc : Allocators)
		{
			Alloc->Clear();

			if (Delete)
			{
				ToDelete.Append(Alloc);
				delete Alloc;
			}
		}

		for (NxFr::Allocator* Alloc : ToDelete)
		{
			Allocators.Remove(Alloc);
		}
	}
}
