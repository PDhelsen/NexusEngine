#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Table)

	void Table::OnLoad(YAML::Node& Node, NxFr::StringView Content)
	{
		Data = NxFr::Yaml::DeserializeFile(Content);
	}

	void Table::OnUnload()
	{
		Data = YAML::Node();
	}

	void Table::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Yaml::SerializeFile(Data, Content);
	}
}
