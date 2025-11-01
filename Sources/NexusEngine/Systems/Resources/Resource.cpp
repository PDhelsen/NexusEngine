#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	Resource::Resource(NxFr::StringView Path)
		: Path(Path), Loaded(false)
	{
	}

	Resource::~Resource()
	{
		NEXUS_ASSERT(!IsLoaded(), Default, "Resource %s was not unloaded", Path.C());
	}

	void Resource::Load(NxFr::StringView FilePath)
	{
		Loaded = true;
	}

	void Resource::Unload()
	{
		Loaded = false;
	}
}
