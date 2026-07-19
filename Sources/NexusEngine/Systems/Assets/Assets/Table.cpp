#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEn
{
	NX_ASSET(Table)

	void Table::OnDraw()
	{
		Asset::OnDraw();
	}

	void Table::OnClone(const Object& Other)
	{
		const Table& Instance = static_cast<const Table&>(Other);

		Data = Instance.Data;
	}

	void Table::OnSave(NxFr::StringView Path) const
	{
		NxFr::Yaml::SerializeAndSave(Data, Path);
	}

	void Table::OnLoad(NxFr::StringView Path)
	{
		Data = NxFr::Yaml::LoadAndDeserialize(Path);
	}

	void Table::OnUnload()
	{
		Data = YAML::Node();
	}
}
