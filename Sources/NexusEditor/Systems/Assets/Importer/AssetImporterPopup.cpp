#include "NexusEditor/Systems/Assets/Importer/AssetImporterPopup.h"

#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"

namespace NxEd
{
	static const NxEn::GUI::Menu::Item* MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/Importer", NxFr::Delegate<void()>([]()
	{
		AssetImporterPopup::ShowWithPath("");
	}));

	const static float ButtonWidth = 100.0f;

	void AssetImporterPopup::ShowWithPath(NxFr::StringView Path)
	{
		static AssetImporterPopup Instance;
		Instance.Initialize();

		Instance.Show();
		Instance.SetPath(Path);
	}

	AssetImporterPopup::AssetImporterPopup()
		: Path(64), Type()
	{
	}

	AssetImporterPopup::~AssetImporterPopup()
	{
	}

	void AssetImporterPopup::OnInitialize()
	{
		Popup::OnInitialize();

		SetTitle("Importer");
		SetMessage("");

		AddButton("Import", { this, &AssetImporterPopup::Import });
		AddButton("Cancel");
	}

	void AssetImporterPopup::OnEnable()
	{
		Popup::OnEnable();

		Type.Clear();
		Path.Clear();

		Style.Reset();
		Style.WidthLabel = ButtonWidth;
	}

	void AssetImporterPopup::OnDraw()
	{
		Style.Width = NxEn::GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Path").x + NxEn::GUI::Styles::WidthButton()), 0, true, true).x - ButtonWidth;
		NxEn::GUI::Drawer<NxFr::String>::Field(Path, "Path", "", &Style);
		ImGui::SameLine();
		if (ImGui::Button("Pick", { NxEn::GUI::Styles::WidthButton(), 0.0f }))
		{
			Pick();
		}

		Style.Width = -1.0f;
		NxEn::GUI::Drawer<NxFr::String>::Field(Type, "Type", "", &Style);
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

	void AssetImporterPopup::Import()
	{
		Type.Validate();
		Path.Validate();

		AssetImporter::Run(Path, NxFr::StringId(Type));
	}
}
