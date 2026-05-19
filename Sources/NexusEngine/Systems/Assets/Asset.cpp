#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	const AssetMetadata& GetMetadata(NxFr::GUID Id)
	{
		return Application::GetSystem<AssetsSystem>()->GetMetadata(Id);
	}

	NX_OBJECT_IMPLEMENTATION(Asset)

	Asset::Asset()
		: Id(0), Dirty(false)
	{
		SetTickable(false);
	}

	Asset::~Asset()
	{
	}

	void Asset::OnGui(float TimeStep)
	{
		const AssetMetadata& Medata = GetMetadata(Id);

		GUI::Drawer<NxFr::GUID>::Property(Id, "Id");
		GUI::Drawer<NxFr::String>::Property(Medata.GetName(), "Name");
		GUI::Drawer<NxFr::String>::Property(Medata.GetAssetPath(), "Path");

		ImGui::Separator();
	}

	YAML::Node Asset::Save(NxFr::StringView ContentFsPath)
	{
		YAML::Node Node;
		OnSave(Node, ContentFsPath);

		Dirty = false;
		return Node;
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
