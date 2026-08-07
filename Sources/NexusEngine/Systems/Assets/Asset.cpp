#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	Asset::Asset()
		: Id(0), Name(), Dirty(false)
	{
		SetTickable(false);
	}

	Asset::~Asset()
	{
	}

	Asset* Asset::Clone() const
	{
		Asset* Instance = AssetsSystem::GetFactory().Create(GetObjectType());
		Instance->Object::Clone(this);
		return Instance;
	}

	void Asset::OnDraw()
	{
		GUI::Drawer<NxFr::GUID>::Property(Id, "Id");
		GUI::Drawer<NxFr::String>::Property(Name, "Name");

		GUI::Draw::Separator();
	}
}
