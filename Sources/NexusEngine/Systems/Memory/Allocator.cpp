#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

#include "NexusEngine/Core/NexusConfig.h"

namespace NxEn
{
	static SettingSeq<uint64>* SettingSizes = SettingSeq<uint64>::Create("Settings", "MemoryAllocatorsSize", { 0, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, NX_MEMORY_ALLOCATOR_SIZE, });

	Allocator::Allocator(AllocatorType Type)
		: NxFr::Allocator(0), Allocators(8, nullptr), Type(Type)
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
		NX_ASSERT(Size < GetAllocatorSize(), Default, "Allocation size requested overflow allocator size");

		NxFr::Allocator* Alloc = GetAllocator(Size, Alignement);

		uint64 Marker = Alloc->UsedAmount();

		void* Pointer = NxFr::Memory::Allocate(Size, Alloc, Alignement);

		uint64 Delta = Alloc->UsedAmount() - Marker;
		IncreaseAmount(Delta);

		return Pointer;
	}

	void* Allocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (!Pointer)
		{
			return nullptr;
		}

		NX_ASSERT(Size < GetAllocatorSize(), Default, "Allocation size requested overflow allocator size");

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

	NxFr::Allocator* Allocator::GetAllocator(uint64 Size, uint64 Alignement)
	{
		if (Type == AllocatorType::Small)
		{
			Size = !NxFr::Math::IsPowerOfTwo(Size) ? NxFr::Math::NextPowerOfTwo(Size) : Size;
		}

		NxFr::Allocator* Result = FindAllocator(Size, Alignement);
		if (Result == nullptr)
		{
			Result = CreateAllocator(GetAllocatorSize(), Size);
		}

		return Result;
	}

	NxFr::Allocator* Allocator::FindAllocator(uint64 Size, uint64 Alignement)
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

	NxFr::Allocator* Allocator::CreateAllocator(uint64 Size, uint64 Stride)
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

		Allocators.Append(Alloc);
		return Alloc;
	}

	void Allocator::ClearAllocators(bool Delete)
	{
		NxFr::Allocator::Scope Context(nullptr);

		for (uint64 Index = 0; Index < Allocators.GetCount(); Index++)
		{
			NxFr::Allocator* Alloc = Allocators[Index];
			Alloc->Clear();

			if (Delete)
			{
				delete Alloc;
			}
		}

		if (Delete)
		{
			Allocators.Clear();
		}
	}

	uint64 Allocator::GetAllocatorSize()
	{
		if (Type == NxEn::AllocatorType::Raw)
		{
			return NxFr::Integer::MaxUI64;
		}

		uint64 Size = NX_MEMORY_ALLOCATOR_SIZE;
		if (SettingSizes != nullptr && SettingSizes->GetValue().IsValidIndex((uint64)Type))
		{
			Size = SettingSizes->GetValue()[(uint64)Type];
		}

		return Size;
	}
}
