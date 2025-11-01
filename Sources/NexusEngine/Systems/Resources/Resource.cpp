#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	Resource::Resource(NxFr::StringView Path, bool Loaded)
		: Path(Path), Loaded(Loaded)
	{
	}

	Resource::~Resource()
	{
		NEXUS_ASSERT(!IsLoaded(), Default, "Resource %s was not unloaded", Path.C());
	}

	void Resource::Save(NxFr::StringView FilePath)
	{
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
