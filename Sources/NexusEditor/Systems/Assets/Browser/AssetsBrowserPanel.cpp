#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	AssetsBrowserPanel::Info::Info(NxFr::StringView Path, uint64 Depth)
		: Path(Path), Label(), Type(), Depth(Depth), Jump(-1), Expand(false), Selected(false)
	{
		Update(Path);
	}

	void AssetsBrowserPanel::Info::Update(NxFr::StringView FilePath)
	{
		Path = FilePath;
		Type = NxFr::Path::IsDirectory(Path) ? InfoType::Directory :
			NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::Extension) ? InfoType::Asset :
			InfoType::File;
		Label = Type == InfoType::Directory ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path);
	}

	AssetsBrowserPanel::AssetsBrowserPanel()
		: Style(), Infos(), Buffer(), Select(), Selected(128), Search(), Filter(128)
	{
		Select.Flag = 0;
	}

	AssetsBrowserPanel::~AssetsBrowserPanel()
	{
	}

	void AssetsBrowserPanel::Refresh()
	{
		FetchFolder();
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		Panel::OnInitialize();

		SetTitle("Assets Browser");
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Panel::OnEnable();

		Style.Reset();
		Style.Width = -1.0f;
		Style.WidthLabel = 0.0f;

		Refresh();
	}

	void AssetsBrowserPanel::OnGui(float TimeStep)
	{
		DrawHeader();

		uint64 Index = 0;
		DrawFolder(Index);

		ApplySelection();
	}

	void AssetsBrowserPanel::DrawHeader()
	{
		if (NxEn::GUI::Drawer<NxFr::String>::Field(Filter, "Filter", "", &Style))
		{
			ApplySearch();
		}
		NxEn::GUI::Drawer<NxFr::String>::Property(Selected, "Selected", &Style);
		ImGui::Separator();
	}

	void AssetsBrowserPanel::FetchFolder(uint64 Depth, NxFr::StringView Path)
	{
		if (Depth == 0)
		{
			Path = NxFr::Paths::Assets;
			Infos.Clear();

			Infos.AppendConstruct(Path, Depth);
			Depth++;
		}

		NxFr::Directory Root = NxFr::Directory(Path);
		for (auto& Item : Root)
		{
			if (NxFr::Path::IsDirectory(Item))
			{
				Info& Instance = Infos.AppendConstruct(Item, Depth);
				FetchFolder(Depth + 1, Instance.Path);
				Instance.Jump = Infos.GetCount();
			}
			else
			{
				Info& Instance = Infos.Last();

				if (NxFr::Path::GetPathWithoutExtension(Item) == NxFr::Path::GetPathWithoutExtension(Instance.Path))
				{
					if (Instance.Type == InfoType::File)
					{
						Instance.Update(Item);
					}
				}
				else
				{
					Infos.AppendConstruct(Item, Depth);
				}
			}
		}
	}

	void AssetsBrowserPanel::DrawFolder(uint64& Index)
	{
		Info& Instance = Infos[Index];
		NxFr::String& Id = GenerateImGuiLabel(Instance);
		bool DrawAsSearch = !Search.IsEmpty();
		
		if (!DrawAsSearch || Search.Contains(Instance.Path))
		{
			// Draw
			uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
				| (Instance.Type != InfoType::Directory || DrawAsSearch ? ImGuiTreeNodeFlags_Leaf : 0)
				| (Instance.Selected ? ImGuiTreeNodeFlags_Selected : 0);

			Instance.Expand = ImGui::TreeNodeEx(Id.C(), Flag);

			// Inputs
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				Select.Flag = NxFr::Integer::SetBit1(Select.Flag, (uint64)0);
				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
				{
					Select.State = !Instance.Selected;
					Select.From = Index;
					Select.To = Index;
				}
				else if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
				{
					Select.State = true;
					Select.To = Index > Select.From ? Index : Select.From;
					Select.From = Index < Select.From ? Index : Select.From;
				}
				else
				{
					Select.State = true;
					Select.From = Index;
					Select.To = Index;
					Select.Flag = NxFr::Integer::SetBit1(Select.Flag, (uint64)1);
				}
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && Instance.Type == InfoType::File)
			{
				NxFr::String Path = NxFr::Path::ConvertAbsoluteToRelative((NxFr::StringView)Instance.Path, NxFr::Paths::Assets);
				AssetImporterPopup::ShowWithPath(Path);
			}
		}

		// Iterate
		Index++;

		if (Instance.Type == InfoType::Directory)
		{
			if (Instance.Expand || DrawAsSearch)
			{
				if (!DrawAsSearch)
				{
					ImGui::TreePush(Id.C());
				}

				while (Index < Infos.GetCount() && Instance.Depth < Infos[Index].Depth)
				{
					DrawFolder(Index);
				}

				if (!DrawAsSearch)
				{
					ImGui::TreePop();
				}
			}
			else
			{
				Index = Instance.Jump;
			}
		}
	}

	NxFr::String& AssetsBrowserPanel::GenerateImGuiLabel(const Info& Instance)
	{
		Buffer = Instance.Type == InfoType::Directory ? "D" : Instance.Type == InfoType::Asset ? "A" : "F";
		Buffer += " ";
		Buffer += Instance.Label;
		Buffer += "##";
		Buffer += Instance.Path;
		return Buffer;
	}

	void AssetsBrowserPanel::ApplySelection()
	{
		if (!NxFr::Integer::CheckBit(Select.Flag, (uint64)0))
		{
			return;
		}

		if (NxFr::Integer::CheckBit(Select.Flag, (uint64)1))
		{
			for (auto& Instance : Infos)
			{
				Instance.Selected = false;
			}
		}

		for (uint64 I = Select.From; I <= Select.To; ++I)
		{
			Infos[I].Selected = Select.State;
		}

		if (Infos.IsValidIndex(Select.From))
		{
			Info& Instance = Infos[Select.From];
			Selected = NxFr::Path::ConvertAbsoluteToRelative((NxFr::StringView)Instance.Path, NxFr::Paths::Assets);
		}

		Select.Flag = 0;
	}

	void AssetsBrowserPanel::ApplySearch()
	{
		Search.Clear();

		// Registry
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxFr::Array<NxFr::GUID> Ids = System->Find(Filter);
		for (auto& Id : Ids)
		{
			NxFr::String Path = System->IdToPath(Id);
			Path += "." + NxEn::AssetMetadata::Extension;
			NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
			Search.Append(NxFr::Move(Path));
		}

		//Folder
		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, " ");
		for (auto& Instance : Infos)
		{
			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				NxFr::StringView Substring = Instance.Path;
				if (!NxFr::StringUtility::Contains(Filters[Index], NxFr::Path::SeparatorDirectory))
				{
					Substring = NxFr::Path::Split(Substring).Last();
				}
				if (NxFr::StringUtility::Contains(Substring, Filters[Index]))
				{
					Search.Append(Instance.Path);
				}
			}
		}
	}
}
