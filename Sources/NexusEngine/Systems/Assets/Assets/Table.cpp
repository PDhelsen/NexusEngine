#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEn
{
	NX_ASSET(Table)

	void Table::OnDraw()
	{
		Asset::OnDraw();
	}

	void Table::OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		NxFr::Yaml::SerializeFile(Data, ContentFsPath);
	}

	void Table::OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		Data = NxFr::Yaml::DeserializeFile(ContentFsPath);
	}

	void Table::OnUnload()
	{
		Data = YAML::Node();
	}

	void Table::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const
	{
	}
}
