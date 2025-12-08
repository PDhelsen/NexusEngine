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

	AssetsBrowserPanel::Info::Info(NxFr::StringView FilePath)
		: Path(""), Label(), Type(), Next(-1), Expand(false), Selected(false)
	{
		Update(FilePath);
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
		: Inputs(nullptr), Style(), Buffer(), Infos(), Select(), Selected(128), Search(), Filter(128)
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

		Inputs = NxEn::Application::GetSystem<NxEn::InputSystem>();

		Style.Reset();
		Style.Width = -1.0f;
		Style.WidthLabel = 0.0f;

		Refresh();
	}

	void AssetsBrowserPanel::OnGui(float TimeStep)
	{
		DrawHeader();

		int64 Index = 0;
		DrawFolder(Index);

		ApplySelection();
	}

	void AssetsBrowserPanel::DrawHeader()
	{
		if (NxEn::GUI::Draw::Button("Refresh", &Style))
		{
			Refresh();
		}

		if (NxEn::GUI::Drawer<NxFr::String>::Field(Filter, "Filter", "", &Style))
		{
			ApplySearch();
		}

		NxEn::GUI::Drawer<NxFr::String>::Property(Selected, "Selected", &Style);

		ImGui::Separator();
	}

	void AssetsBrowserPanel::FetchFolder()
	{
		NxFr::Directory Root = NxFr::Directory(NxFr::Paths::Assets);
		NxFr::List<NxFr::String> Content = Root.GetContent(true);
		NxFr::Stack<Info*> Directories;

		Infos.Clear();
		Infos.Reserve(Content.GetCount());
		Infos.AppendConstruct(Root.GetPath());
		Directories.Append(&Infos.Last());

		for (auto& Item : Content)
		{
			Info* Last = &Infos.Last();
			if (NxFr::Path::GetPathWithoutExtension(Last->Path) == NxFr::Path::GetPathWithoutExtension(Item))
			{
				if (Last->Type == InfoType::File)
				{
					Last->Update(Item);
				}

				continue;
			}

			Info* Instance = &Infos.AppendConstruct(Item);
			Last->Next = Infos.GetCount() - 1;

			if (Instance->Type == InfoType::Directory)
			{
				while (!Directories.IsEmpty())
				{
					Last = Directories.Get();
					
					if (NxFr::Path::Split(Last->Path).GetCount() < NxFr::Path::Split(Instance->Path).GetCount())
					{
						break;
					}

					Directories.Remove();
					Last->Next = Infos.GetCount() - 1;
				}

				Directories.Append(Instance);
			}
		}

		while (!Directories.IsEmpty())
		{
			Info* Instance = Directories.Get();
			Directories.Remove();

			Instance->Next = Infos.GetCount();
		}
	}

	void AssetsBrowserPanel::DrawFolder(int64& Index)
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
			if (ImGui::IsItemHovered())
			{
				if (Inputs->CheckButton(NxEn::Input::Button::MouseLeft))
				{
					Select.Flag = NxFr::Integer::SetBit1(Select.Flag, (uint64)0);
					if (Inputs->CheckModifier(NxEn::Input::Modifier::Ctrl))
					{
						Select.State = !Instance.Selected;
						Select.From = Index;
						Select.To = Index;
					}
					else if (Inputs->CheckModifier(NxEn::Input::Modifier::Shift))
					{
						Select.State = true;
						Select.To = (uint64)Index > Select.From ? Index : Select.From;
						Select.From = (uint64)Index < Select.From ? Index : Select.From;
					}
					else
					{
						Select.State = true;
						Select.From = Index;
						Select.To = Index;
						Select.Flag = NxFr::Integer::SetBit1(Select.Flag, (uint64)1);
					}
				}
				if (Inputs->CheckButton(NxEn::Input::Button::MouseRight) && Instance.Type != InfoType::Directory)
				{
					NxFr::String Path = NxFr::Path::ConvertAbsoluteToRelative((NxFr::StringView)Instance.Path, NxFr::Paths::Assets);
					AssetImporterPopup::ShowWithPath(Path);
				}
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

				while (Index < Instance.Next)
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
				Index = Instance.Next;
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

		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxFr::Array<NxFr::GUID> Ids = System->Find(Filter);
		for (auto& Id : Ids)
		{
			NxFr::String Path = System->IdToPath(Id);
			Path += "." + NxEn::AssetMetadata::Extension;
			NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
			Search.Append(NxFr::Move(Path));
		}
	}
}
