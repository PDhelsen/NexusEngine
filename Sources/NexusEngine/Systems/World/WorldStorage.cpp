#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldStorage.h"

namespace NxEn
{
	NxFr::Factory<WorldStorage>& WorldStorage::GetFactory()
	{
		NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::Constant);

		static NxFr::Factory<WorldStorage> Factory;
		return Factory;
	}
}
