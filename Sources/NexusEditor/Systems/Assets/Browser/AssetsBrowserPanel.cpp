#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	const static NxEn::Command CmdAssetPing = NxEn::Command::Create("Assets.Ping"_Sid, "Ping path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
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
		Selection.Clear();
		Selected = nullptr;
		Filtered.Clear();
		Filter.Clear();

		FetchFolder();
	}

	void AssetsBrowserPanel::Select(NxFr::StringView Path, bool Additive, bool List)
	{
		Select(PathToId(Path), Additive, List);
	}

	void AssetsBrowserPanel::Select(NxFr::GUID Id, bool Additive, bool List)
	{
		Select(Map[Id], Additive, List);
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
		DrawItem(Items);
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

		NxEn::GUI::Drawer<NxFr::String>::Property(Selected ? Selected->GetPath() : "", "Selected", &Style);

		ImGui::Separator();
	}

	void AssetsBrowserPanel::DrawItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		bool Browse = Filter.IsEmpty();
		bool ExpandChanged = false;
		if (Browse || Filtered.Contains(Item))
		{
			// Draw
			uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
				| (Item->IsLeaf() ? ImGuiTreeNodeFlags_Leaf : 0)
				| (Item->IsSelected() ? ImGuiTreeNodeFlags_Selected : 0);

			if (Browse)
			{
				ImGui::SetNextItemOpen(Item->Expanded, ImGuiCond_Always);
				bool State = ImGui::TreeNodeEx(Item->ImGuiText.C(), Flag);
				ExpandChanged = Item->Expanded != State;
				Item->Expanded = State;
			}
			else
			{
				ImGui::Selectable(Item->ImGuiText.C(), Item->Selected);
			}

			// Inputs
			if (ImGui::IsItemHovered())
			{
				if (Inputs->CheckButton(NxEn::Input::Button::MouseLeft, NxEn::Input::State::Pressed) && !ExpandChanged)
				{
					SelectItem(Item);
				}
			}
		}

		// Iterate
		if (Item->GetObjectType() == AssetsBrowserItemDirectory::GetClassType() && Item->IsOpen())
		{
			if (Browse)
			{
				ImGui::TreePush(Item->ImGuiText.C());
			}

			DrawItem(Item->Child);

			if (Browse)
			{
				ImGui::TreePop();
			}
		}

		DrawItem(Item->Next);
	}

	void AssetsBrowserPanel::SelectItem(AssetsBrowserItem* Item)
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

	void AssetsBrowserPanel::FetchFolder()
	{
		Map.Clear();
		Items = FetchItems(NxFr::Paths::Assets, nullptr);
		PurgeDuplicates(Items);
	}

	AssetsBrowserItem* AssetsBrowserPanel::FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent)
	{
		NxFr::String P = FileToPath(Path);
		NxFr::GUID Id = PathToId(P);

		AssetsBrowserItem* Instance = AppendItem(Id, P);

		if (Parent)
		{
			Instance->Parent = Parent;
			if (Parent->Child == nullptr)
			{
				Parent->Child = Instance;
			}
		}

		if (Instance->GetObjectType() == AssetsBrowserItemDirectory::GetClassType())
		{
			AssetsBrowserItem* Previous = nullptr;

			NxFr::Directory Root(Path);
			for (auto& Path : Root)
			{
				AssetsBrowserItem* Child = FetchItems(Path, Instance);
				if (Previous)
				{
					Child->Previous = Previous;
					Previous->Next = Child;
				}
				Previous = Child;
			}
		}

		return Instance;
	}

	AssetsBrowserItem* AssetsBrowserPanel::PurgeDuplicates(AssetsBrowserItem* Item)
	{
		if (Item == nullptr)
		{
			return nullptr;
		}

		PurgeDuplicates(Item->Child);

		if (Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
		{
			if (Item->Next && Item->Name == Item->Next->Name)
			{
				RemoveItem(Item->Next);
			}
			else if (Item->Previous && Item->Name == Item->Previous->Name)
			{
				RemoveItem(Item->Previous);
			}
		}

		while (Item)
		{
			Item = PurgeDuplicates(Item->Next);
		}

		return Item ? Item->Next : nullptr;
	}

	AssetsBrowserItem* AssetsBrowserPanel::AppendItem(NxFr::GUID Id, NxFr::StringView Path)
	{
		AssetsBrowserItem* Item = nullptr;

		if (NxFr::Path::IsDirectory(Path))
		{
			Item = new AssetsBrowserItemDirectory(Id, Path);
		}
		else
		{
			if (NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::Extension))
			{
				Item = new AssetsBrowserItemAsset(Id, Path);
			}
			else
			{
				Item = new AssetsBrowserItemFile(Id, Path);
			}
		}

		Map.Append(Item->GetId(), Item);
		return Item;
	}

	void AssetsBrowserPanel::RemoveItem(AssetsBrowserItem* Item)
	{
		if (Item->Parent)
		{
			if (Item->Parent->Child == Item)
			{
				Item->Parent->Child = Item->Next;
			}
		}
		if (Item->Previous)
		{
			Item->Previous->Next = Item->Next;
		}
		if (Item->Next)
		{
			Item->Next->Previous = Item->Previous;
		}

		while (Item->Child)
		{
			RemoveItem(Item->Child);
		}

		Map.Remove(Item->GetId());
		delete Item;
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
			NxFr::StringView P1 = Item->GetPath();
			NxFr::StringView P2 = Selected->GetPath();
			AssetsBrowserItem* I1 = P1 < P2 ? Item : Selected;
			AssetsBrowserItem* I2 = P1 >= P2 ? Item : Selected;
			while (I1 && I1 != I2)
			{
				if (IsVisible(I1))
				{
					I1->Selected = Item->Selected;
					Selection.Append(I1);
				}

				I1 = I1->GetIterator();
			}
			if (I1)
			{
				if (IsVisible(I1))
				{
					I1->Selected = Item->Selected;
					Selection.Append(I1);
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
			AssetsBrowserItem* Item = Map[Id];
			Show(Item);
			Filtered.Append(Item);
		}

		// File and Directory
		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, "");
		AssetsBrowserItem* Item = Items;
		while (Item)
		{
			if (Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
			{
				Item = Item->GetIterator();
				continue;
			}

			for (auto& F : Filters)
			{
				if (NxFr::StringUtility::Contains(Item->GetPath(), F))
				{
					Show(Item);
					Filtered.Append(Item);
					break;
				}
			}

			Item = Item->GetIterator();
		}
	}

	NxFr::GUID AssetsBrowserPanel::PathToId(NxFr::StringView Path)
	{
		NxFr::GUID Id = NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::Extension) ? 
			Assets->PathToId(NxFr::Path::GetPathWithoutExtension(Path)) :
			NxFr::Hash<>::HashObject(Path);

		return Id;
	}

	NxFr::String AssetsBrowserPanel::FileToPath(NxFr::StringView Path)
	{
		if (Path == NxFr::Paths::Assets)
		{
			return "Assets/";
		}

		return NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::PathToFile(NxFr::StringView Path)
	{
		if (Path == "Assets/")
		{
			return NxFr::Paths::Assets.Data;
		}

		return NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
	}
}
