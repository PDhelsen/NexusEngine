#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	AssetImporterPopup* PopupInstance = NxEn::Object::Create<AssetImporterPopup>(false);

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Importer", NxFr::Delegate<void()>([]()
	{
		PopupInstance->Show();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetImporterPopup)

	void AssetImporterPopup::ShowWithPath(NxFr::StringView Path)
	{
		PopupInstance->Show();
		PopupInstance->Path = Path;
	}

	AssetImporterPopup::AssetImporterPopup()
		: Type(), Path(64), Release(true)
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
		Style.WidthLabel = 100.0f;
		Style.Flag = ImGuiInputTextFlags_EnterReturnsTrue;
	}

	void AssetImporterPopup::OnGui(float TimeStep)
	{
		Style.Width = -1.0f;
		NxEn::GUI::Drawer<NxFr::String>::Field(Type, "Type", "##Type", &Style);

		Style.Width = NxEn::GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Path").x + NxEn::GUI::Style::GetVar(NxEn::GUI::Style::IdWidthButton)), 1, false, true).x - 100.0f;
		NxEn::GUI::Drawer<NxFr::String>::Field(Path, "Path", "##Path", &Style);
		ImGui::SameLine();
		if (ImGui::Button("Pick", { NxEn::GUI::Style::GetVar(NxEn::GUI::Style::IdWidthButton), 0.0f }))
		{
			Pick();
		}

		Style.Width = -1.0f;
		NxEn::GUI::Drawer<bool>::Field(Release, "Release", "##Release", &Style);
	}

	void AssetImporterPopup::Pick()
	{
		NxFr::String Selection = NxFr::Platform::GetInstance()->OpenFileDialog("Pick file", "", "", NxFr::Paths::Assets);
		if (Selection.IsEmpty())
		{
			return;
		}

		Path = NxFr::Path::ConvertAbsoluteToRelative((NxFr::StringView)Selection, NxFr::Paths::Assets);
	}

	void AssetImporterPopup::Import()
	{
		Type.Validate();
		Path.Validate();

		AssetImporter::Import(NxFr::StringId(Type), Path, Release);
	}
}
