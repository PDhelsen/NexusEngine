#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Asset)

	Asset::Asset()
		: Id(0), Dirty(false)
	{
		SetTickable(false);
	}

	Asset::~Asset()
	{
	}

	YAML::Node Asset::Save(NxFr::StringView Content)
	{
		YAML::Node Node;
		OnSave(Node, Content);

		Dirty = false;
		return Node;
	}

	void Asset::Load(const YAML::Node& Node, NxFr::StringView Content)
	{
		OnLoad(Node, Content);
	}

	void Asset::Unload()
	{
		OnUnload();
	}
}
