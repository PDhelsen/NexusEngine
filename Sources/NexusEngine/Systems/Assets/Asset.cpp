#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	Asset::Asset()
		: Id(0), Dirty(false)
	{
		SetTickable(false);
	}

	Asset::~Asset()
	{
	}

	void Asset::OnDraw()
	{
		GUI::Drawer<NxFr::GUID>::Property(Id, "Id");
		GUI::Drawer<NxFr::String>::Property(Name, "Name");

		ImGui::Separator();
	}

	void Asset::Save(YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		OnSave(Node, ContentFsPath);
	}

	void Asset::Load(const YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		OnLoad(Node, ContentFsPath);
	}

	void Asset::Unload()
	{
		OnUnload();
	}
}
