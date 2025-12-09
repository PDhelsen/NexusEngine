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

	const static NxEn::Command CmdAssetImport = NxEn::Command::Create("Assets.Ping"_Sid, "Ping path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Select(Path);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	AssetsBrowserPanel::AssetsBrowserPanel()
		: Style(), Assets(nullptr),Inputs(nullptr), Items(), Map(), Selection(), Selected(nullptr), Filter(64)
	{
	}

	AssetsBrowserPanel::~AssetsBrowserPanel()
	{
	}

	void AssetsBrowserPanel::Refresh()
	{
		FetchFolder();
	}

	void AssetsBrowserPanel::Select(NxFr::GUID Id, bool Additive, bool List)
	{
		Select(&Items[Map[Id]], Additive, List);
	}

	void AssetsBrowserPanel::Select(NxFr::StringView Path, bool Additive, bool List)
	{
		NxFr::GUID Id = Assets->PathToId(Path);
		if (Id == 0)
		{
			Id = NxFr::Hash<>::HashObject(Path);
		}

		Select(Id, Additive, List);
	}

	void AssetsBrowserPanel::Find(NxFr::StringView Query)
	{
		Filter.Clear();
		Filter += Query;

		Find();
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		Panel::OnInitialize();

		SetTitle("Assets Browser");
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Panel::OnEnable();

		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Inputs = NxEn::Application::GetSystem<NxEn::InputSystem>();

		Style.Reset();
		Style.Width = -1.0f;
		Style.WidthLabel = 0.0f;

		Refresh();
	}

	void AssetsBrowserPanel::OnGui(float TimeStep)
	{
		DrawHeader();
		DrawItem(&Items[0]);
	}

	void AssetsBrowserPanel::DrawHeader()
	{
		if (NxEn::GUI::Draw::Button("Refresh", &Style))
		{
			Refresh();
		}

		if (NxEn::GUI::Drawer<NxFr::String>::Field(Filter, "Filter", "", &Style))
		{
			Find();
		}

		NxEn::GUI::Drawer<NxFr::String>::Property(Selected ? Selected->Path : "", "Selected", &Style);

		ImGui::Separator();
	}

	void AssetsBrowserPanel::DrawItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		bool Browse = Filter.IsEmpty();
		if (Browse || Filtered.Contains(Item))
		{
			// Draw
			uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
				| (Item->ItemType != AssetsBrowserItem::Type::Directory ? ImGuiTreeNodeFlags_Leaf : 0)
				| (Item->Selected ? ImGuiTreeNodeFlags_Selected : 0);

			if (Browse)
			{
				ImGui::SetNextItemOpen(Item->Expanded, ImGuiCond_Always);
				Item->Expanded = ImGui::TreeNodeEx(Item->ImGuiText.C(), Flag);
			}
			else
			{
				ImGui::Selectable(Item->ImGuiText.C(), Item->Selected);
			}

			// Inputs
			if (ImGui::IsItemHovered())
			{
				if (Inputs->CheckButton(NxEn::Input::Button::MouseLeft))
				{
					if (Inputs->CheckModifier(NxEn::Input::Modifier::Ctrl))
					{
						Select(Item, true);
					}
					else if (Inputs->CheckModifier(NxEn::Input::Modifier::Shift))
					{
						Select(Item, true, true);
					}
					else
					{
						Select(Item);
					}
				}
			}
		}

		// Iterate
		if (Item->ItemType == AssetsBrowserItem::Type::Directory && Item->Expanded)
		{
			if (Browse) ImGui::TreePush(Item->ImGuiText.C());
			DrawItem(Item->Child);
			if (Browse) ImGui::TreePop();
		}

		DrawItem(Item->Next);
	}

	void AssetsBrowserPanel::FetchFolder()
	{
		NxFr::Directory Root = NxFr::Directory(NxFr::Paths::Assets);
		NxFr::List<NxFr::String> Content = Root.GetContent(true);
		NxFr::Stack<AssetsBrowserItem*> Directories;

		Items.Clear();
		Items.Reserve(Content.GetCount());
		Map.Clear();
		Map.Reserve(Content.GetCount());

		AssetsBrowserItem* Item = AppendItem("Assets/");
		Item->Depth = 0;
		Directories.Append(Item);

		for (auto& Path : Content)
		{
			NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);

			AssetsBrowserItem* Last = &Items.Last();
			if (Last->GetPathWithoutExtension() == NxFr::Path::GetPathWithoutExtension(Path))
			{
				if (Last->GetType() == AssetsBrowserItem::Type::File)
				{
					AppendItem(Path, true);
				}

				continue;
			}

			AssetsBrowserItem* Item = AppendItem(Path);

			if (Last->Depth == Item->Depth)
			{
				Last->Next = Item;
			}
			else if (Last->Depth < Item->Depth)
			{
				Last->Child = Item;
			}
			else if (Last->Depth > Item->Depth)
			{
				while (!Directories.IsEmpty() && Last->Depth > Item->Depth)
				{
					Last = Directories.Get();
					Directories.Remove();
				}

				Last->Next = Item;
			}

			Item->Parent = Directories.Get();
			if (Item->GetType() == AssetsBrowserItem::Type::Directory)
			{
				Directories.Append(Item);
			}
		}
	}

	AssetsBrowserItem* AssetsBrowserPanel::AppendItem(NxFr::StringView Path, bool Replace)
	{
		NxFr::GUID Id = Assets->PathToId(NxFr::Path::GetPathWithoutExtension(Path));
		if (Id == 0)
		{
			Id = NxFr::Hash<>::HashObject(Path);
		}

		if (!Replace)
		{
			Map.Append(Id, Items.GetCount());
		}

		return Replace ?
			&Items.AssignConstruct(Items.GetCount() - 1, Id, Path) :
			&Items.AppendConstruct(Id, Path);
	}

	void AssetsBrowserPanel::Select(AssetsBrowserItem* Item, bool Additive, bool List)
	{
		if (!Additive)
		{
			for (auto& Item : Selection)
			{
				Item->Selected = false;
			}

			Selection.Clear();
			Selected = nullptr;
		}

		if (!Item)
		{
			return;
		}

		Item->Selected = !Item->Selected;
		Show(Item);
		
		if (List)
		{
			uint64 Start = Map[Item->GetId()];
			uint64 End = Selected != nullptr ? Map[Selected->GetId()] : Start;
			if (Start > End)
			{
				uint64 Temp = Start;
				Start = End;
				End = Temp;
			}

			for (uint64 Index = Start; Index <= End; ++Index)
			{
				AssetsBrowserItem* I = &Items[Index];
				if (IsVisible(I))
				{
					I->Selected = Item->Selected;
					Selection.Append(I);
				}
			}
		}

		Selected = Item;
		Selection.Append(Selected);
	}

	void AssetsBrowserPanel::Show(AssetsBrowserItem* Item)
	{
		AssetsBrowserItem* Parent = Item->Parent;
		while (Parent)
		{
			Parent->Expanded = true;
			Parent = Parent->Parent;
		}
	}

	bool AssetsBrowserPanel::IsVisible(AssetsBrowserItem* Item)
	{
		bool Visible = true;
		AssetsBrowserItem* Parent = Item->Parent;

		while (Parent)
		{
			Visible &= Parent->Expanded;
			Parent = Parent->Parent;
		}

		return Visible;
	}

	void AssetsBrowserPanel::Find()
	{
		Filtered.Clear();

		if (Filter.IsEmpty())
		{
			return;
		}

		// Assets
		NxFr::Array<NxFr::GUID> Ids = Assets->Find(Filter);
		for (auto& Id : Ids)
		{
			AssetsBrowserItem* Item = &Items[Map[Id]];
			Show(Item);
			Filtered.Append(Item);
		}

		// File and Directory
		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, "");
		for (auto& Item : Items)
		{
			if (Item.GetType() == AssetsBrowserItem::Type::Asset)
			{
				continue;
			}

			for (auto& F : Filters)
			{
				if (NxFr::StringUtility::Contains(Item.GetPath(), F))
				{
					Show(&Item);
					Filtered.Append(&Item);
					break;
				}
			}
		}
	}
}
