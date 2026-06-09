#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	Resource::Resource(NxFr::StringView Path)
		: Path(Path)
	{
	}

	Resource::~Resource()
	{
	}
}
