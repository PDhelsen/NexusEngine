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

	void Asset::Load(YAML::Node& Node, NxFr::StringView Content)
	{
		YAML::Node Data = Node["Data"];
		OnLoad(Data, Content);
	}

	void Asset::Unload()
	{
		OnUnload();
	}

	void Asset::Save(YAML::Node& Node, NxFr::StringView Content)
	{
		YAML::Node Data = YAML::Node();
		OnSave(Data, Content);

		Node["Data"] = Data;
		Dirty = false;
	}
}
