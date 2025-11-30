#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetMetadata.h"

namespace NxEn
{
	AssetMetadata::AssetMetadata()
		: Id(0), Type(0), Path("")
	{
	}

	AssetMetadata::AssetMetadata(Asset* Instance, NxFr::StringView Path)
		: Id(Instance->Id), Type(Instance->GetObjectType()), Path(Path)
	{
	}

	AssetMetadata::~AssetMetadata()
	{
	}
}
