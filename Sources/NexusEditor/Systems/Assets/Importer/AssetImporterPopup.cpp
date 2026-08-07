#include "NexusEditor/Systems/Assets/Importer/AssetImporterPopup.h"

#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"

namespace NxEd
{
	static const NxEn::GUI::Menu::Item* MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/Importer", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::GUISystem>()->TryReuseElement<AssetImporterPopup>();
	}));

	AssetImporterPopup::AssetImporterPopup()
		: Path(64), Type()
	{
	}

	AssetImporterPopup::~AssetImporterPopup()
	{
	}

	void AssetImporterPopup::SetPath(NxFr::StringView FilePath)
	{
		if (FilePath.IsEmpty())
		{
			Path.Clear();
			Type.Clear();
			return;
		}

		Path = NxFr::Path::IsRelative(FilePath) ? (NxFr::String)FilePath : NxFr::Path::MakeRelative(FilePath, NxFr::Globals::Paths::Assets);
		if (Type.IsEmpty())
		{
			NxFr::StringId* TypeId = AssetImporter::GetTypes().TryGet(NxFr::Path::GetExtension(Path));
			Type = TypeId ? (NxFr::String)TypeId->GetString() : NxFr::StringUtility::Empty;
		}
	}

	void AssetImporterPopup::OnInitialize()
	{
		Popup::OnInitialize();

		SetNameId("Importer");
		SetMessage("");

		AddButton("Import", { this, &AssetImporterPopup::Import });
		AddButton("Pick", { this, &AssetImporterPopup::Pick });
		AddButton("Cancel");
	}

	void AssetImporterPopup::OnEnable()
	{
		Popup::OnEnable();

		Type.Clear();
		Path.Clear();
	}

	void AssetImporterPopup::OnDraw()
	{
		NxEn::GUI::Transform Visual;

		NxEn::GUI::Drawer<NxFr::String>::Field(Path, "Path", "", Visual);
		NxEn::GUI::Drawer<NxFr::String>::Field(Type, "Type", "", Visual);
	}

	void AssetImporterPopup::Pick()
	{
		NxFr::String Selection = NxFr::Globals::PlatformTarget->OpenFileDialog("Pick file", "", "", NxFr::Globals::Paths::Assets);
		if (Selection.IsEmpty())
		{
			return;
		}

		SetPath(Selection);
	}

	void AssetImporterPopup::Import()
	{
		Type.Validate();
		Path.Validate();

		AssetImporter::Run(Path, NxFr::StringId(Type));
	}
}
