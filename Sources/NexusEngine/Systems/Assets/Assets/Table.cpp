#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEn
{
	NEXUS_ASSET_IMPLEMENTATION(Table)

	void Table::OnSave(YAML::Node& Node, NxFr::StringView ContentFilePath)
	{
		NxFr::Yaml::SerializeFile(Data, ContentFilePath);
	}

	void Table::OnLoad(const YAML::Node& Node, NxFr::StringView ContentFilePath)
	{
		Data = NxFr::Yaml::DeserializeFile(ContentFilePath);
	}

	void Table::OnUnload()
	{
		Data = YAML::Node();
	}

	void Table::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids)
	{
	}
}
