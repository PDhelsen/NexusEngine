#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEn
{
	NEXUS_ASSET_IMPLEMENTATION(Table)

	void Table::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Yaml::SerializeFile(Data, Content);
	}

	void Table::OnLoad(const YAML::Node& Node, NxFr::StringView Content)
	{
		Data = NxFr::Yaml::DeserializeFile(Content);
	}

	void Table::OnUnload()
	{
		Data = YAML::Node();
	}

	void Table::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids)
	{
	}
}
