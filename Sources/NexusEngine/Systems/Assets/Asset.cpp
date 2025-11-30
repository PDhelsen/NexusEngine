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

	void Asset::Load(YAML::Node& Node)
	{
		YAML::Node Data = Node["Data"];
		OnLoad(Data);
	}

	void Asset::Unload()
	{
		OnUnload();
	}

	void Asset::Save(YAML::Node& Node)
	{
		YAML::Node Data = YAML::Node();
		OnSave(Data);

		Node["Data"] = Data;
		Dirty = false;
	}
}
