#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Browser", NxFr::Delegate<void()>([]()
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
		: Infos(), Select(), Buffer()
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

		SetTitle("AssetsBrowser");
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Panel::OnEnable();

		Refresh();
	}

	void AssetsBrowserPanel::OnGui(float TimeStep)
	{
		uint64 Index = 0;
		DrawFolder(Index);
		ApplySelection();
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
		uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
			| (Instance.Type != InfoType::Directory ? ImGuiTreeNodeFlags_Leaf : 0)
			| (Instance.Selected ? ImGuiTreeNodeFlags_Selected : 0);

		Instance.Expand = ImGui::TreeNodeEx(Id.C(), Flag);

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

		Index++;

		if (Instance.Type == InfoType::Directory)
		{
			if (Instance.Expand)
			{
				ImGui::TreePush(Id.C());

				while (Index < Infos.GetCount() && Instance.Depth < Infos[Index].Depth)
				{
					DrawFolder(Index);
				}

				ImGui::TreePop();
			}
			else
			{
				Index = Instance.Jump;
			}
		}
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

		Select.Flag = 0;
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
}
