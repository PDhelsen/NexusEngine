#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/WorldStorage.h"

namespace NxEn
{
	NxFr::Factory<WorldStorage>& WorldStorage::GetFactory()
	{
		static NxFr::Factory<WorldStorage> Factory;
		return Factory;
	}
}
