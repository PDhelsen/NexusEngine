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
	const static NxEn::Command CmdAssetCreate = NxEn::Command::Create("Assets.Create"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Type)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Create(Path, NxFr::StringId(Type));
	}));
	const static NxEn::Command CmdAssetMove = NxEn::Command::Create("Assets.Move"_Sid, "Move path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Move(Path, Target);
	}));
	const static NxEn::Command CmdAssetDelete = NxEn::Command::Create("Assets.Delete"_Sid, "Delete path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Delete(Path);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	AssetsBrowserPanel::AssetsBrowserPanel()
		: Style(), Assets(nullptr),Inputs(nullptr), Items(), Map(), Selection(), Selected(nullptr), Filter(64)
	{
		AssetsBrowserItem::Update = { this, &AssetsBrowserPanel::UpdateItem };
		AssetsBrowserItem::PathToFile = { this, &AssetsBrowserPanel::PathToFile };
	}

	AssetsBrowserPanel::~AssetsBrowserPanel()
	{
	}

	void AssetsBrowserPanel::Refresh()
	{
		Clear();
		Fetch();
	}

	void AssetsBrowserPanel::Find(NxFr::StringView Query)
	{
		Filter.Clear();
		Filter += Query;

		Find();
	}

	void AssetsBrowserPanel::Select(NxFr::GUID Id, bool Additive, bool List)
	{
		Select(Map[Id], Additive, List);
	}

	void AssetsBrowserPanel::Select(NxFr::StringView Path, bool Additive, bool List)
	{
		Select(PathToId(Path), Additive, List);
	}

	void AssetsBrowserPanel::Create(NxFr::StringView Path, NxFr::StringId Type)
	{
		NxFr::String P = ConvertPath(Path);

		AssetsBrowserItem* Item = AppendItem(P, false);
		Item->Create(Path, Type);
		UpdateItem(Item, P, true);
		AttachItem(Item, GetParent(P), true);
	}

	void AssetsBrowserPanel::Move(NxFr::StringView Path, NxFr::StringView Target)
	{
		NxFr::GUID Id = PathToId(Path);

		AssetsBrowserItem* Item = Map[Id];
		DetachItem(Item, true);
		Item->Move(Target);
		AttachItem(Item, GetParent(Item->Path), true);
	}

	void AssetsBrowserPanel::Delete(NxFr::StringView Path)
	{
		NxFr::GUID Id = PathToId(Path);

		AssetsBrowserItem* Item = Map[Id];
		Item->Delete();
		DetachItem(Item, true);
		RemoveItem(Item, true);
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
		bool ExpandChanged = false;

		if (Browse || Filtered.Contains(Item))
		{
			// Draw
			uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
				| (!Item->Child ? ImGuiTreeNodeFlags_Leaf : 0)
				| (Item->Selected ? ImGuiTreeNodeFlags_Selected : 0);

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

	void AssetsBrowserPanel::Clear()
	{
		Selection.Clear();
		Selected = nullptr;
		Filtered.Clear();
		Filter.Clear();

		RemoveItem(Items, false);
		Map.Clear();
		Items = nullptr;
	}

	void AssetsBrowserPanel::Fetch()
	{
		Items = FetchItems(NxFr::Paths::Assets, nullptr);
		PurgeDuplicates(Items);
	}

	AssetsBrowserItem* AssetsBrowserPanel::FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent)
	{
		NxFr::String P = FileToPath(Path);

		AssetsBrowserItem* Item = AppendItem(P, false);
		UpdateItem(Item, P, true);

		if (Parent)
		{
			Item->Parent = Parent;
			if (Parent->Child == nullptr)
			{
				Parent->Child = Item;
			}
		}

		if (Item->GetObjectType() == AssetsBrowserItemDirectory::GetClassType())
		{
			AssetsBrowserItem* Previous = nullptr;

			NxFr::Directory Directory(Path);
			for (auto& Path : Directory)
			{
				AssetsBrowserItem* Child = FetchItems(Path, Item);
				if (Previous)
				{
					Child->Previous = Previous;
					Previous->Next = Child;
				}

				Previous = Child;
			}
		}

		return Item;
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
			if (Item->Next && Item->GetPrettyName() == Item->Next->GetPrettyName())
			{
				RemoveItem(Item->Next, true);
			}
			else if (Item->Previous && Item->GetPrettyName() == Item->Previous->GetPrettyName())
			{
				RemoveItem(Item->Previous, true);
			}
		}

		while (Item)
		{
			Item = PurgeDuplicates(Item->Next);
		}

		return Item ? Item->Next : nullptr;
	}

	AssetsBrowserItem* AssetsBrowserPanel::AppendItem(NxFr::StringView Path, bool AppendId)
	{
		AssetsBrowserItem* Item = nullptr;
		if (NxFr::Path::IsDirectory(Path))
		{
			Item = new AssetsBrowserItemDirectory();
		}
		else
		{
			if (NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::AssetExtension))
			{
				Item = new AssetsBrowserItemAsset();
			}
			else
			{
				Item = new AssetsBrowserItemFile();
			}
		}

		if (AppendId)
		{
			NxFr::GUID Id = PathToId(Path);
			Map.Append(Id, Item);
		}

		return Item;
	}

	void AssetsBrowserPanel::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool UpdateId)
	{
		if (UpdateId)
		{
			if (Map.ContainsKey(Item->Id))
			{
				Map.Remove(Item->Id);
			}

			Item->Id = PathToId(Path);
			Map.Append(Item->Id, Item);
		}

		Item->Path = Path;
		Item->ImGuiText = Item->GetPrefix() + " " + Item->GetPrettyName() + "##" + NxFr::StringUtility::ToString(Item->Id);
	}

	void AssetsBrowserPanel::RemoveItem(AssetsBrowserItem* Item, bool RemoveId)
	{
		if (!Item)
		{
			return;
		}

		DetachItem(Item, false);
		while (Item->Child)
		{
			RemoveItem(Item->Child, RemoveId);
		}

		if (RemoveId)
		{
			Map.Remove(Item->Id);
		}
		else
		{
			Map[Item->Id] = nullptr;
		}

		delete Item;
	}

	void AssetsBrowserPanel::AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent, bool Sort)
	{
		if (!Parent)
		{
			return;
		}

		Item->Parent = Parent;
		Item->Previous = nullptr;
		Item->Next = Parent->Child;

		Parent->Child->Previous = Item;
		Parent->Child = Item;

		if (Sort)
		{
			SortItem(Parent);
		}
	}

	void AssetsBrowserPanel::DetachItem(AssetsBrowserItem* Item, bool Sort)
	{
		if (!Item)
		{
			return;
		}

		if (Item->Previous)
		{
			Item->Previous->Next = Item->Next;
		}
		if (Item->Next)
		{
			Item->Next->Previous = Item->Previous;
		}
		if (Item->Parent)
		{
			if (Item->Parent->Child == Item)
			{
				Item->Parent->Child = Item->Next;
			}

			if (Sort)
			{
				SortItem(Item->Parent);
			}
		}
	}

	void AssetsBrowserPanel::SortItem(AssetsBrowserItem* Item)
	{
		NxFr::Delegate<AssetsBrowserItem* (AssetsBrowserItem*)> Split = [](AssetsBrowserItem* Head)
		{
			AssetsBrowserItem* Slow = Head;
			AssetsBrowserItem* Fast = Head->Next;

			while (Fast && Fast->Next) {
				Slow = Slow->Next;
				Fast = Fast->Next->Next;
			}

			AssetsBrowserItem* second = Slow->Next;
			Slow->Next = nullptr;
			return second;
		};
		NxFr::Delegate<AssetsBrowserItem* (AssetsBrowserItem*, AssetsBrowserItem*)> Merge = [&](AssetsBrowserItem* A, AssetsBrowserItem* B)
		{
			if (!A) return B;
			if (!B) return A;

			if (A->GetPrettyName() <= B->GetPrettyName())
			{
				A->Next = Merge(A->Next, B);
				if (A->Next)
					A->Next->Previous = A;
				A->Previous = nullptr;
				return A;
			}
			else
			{
				B->Next = Merge(A, B->Next);
				if (B->Next)
					B->Next->Previous = B;
				B->Previous = nullptr;
				return B;
			}
		};
		NxFr::Delegate<AssetsBrowserItem* (AssetsBrowserItem*)> MergeSort = [&](AssetsBrowserItem* Item)
		{
			if (!Item || !Item->Next)
				return Item;

			AssetsBrowserItem* Second = Split(Item);

			Item = MergeSort(Item);
			Second = MergeSort(Second);

			return Merge(Item, Second);
		};

		Item->Child = MergeSort.Invoke(Item->Child);
	}

	AssetsBrowserItem* AssetsBrowserPanel::GetParent(NxFr::StringView Path)
	{
		NxFr::StringView Directory = NxFr::Path::GetParent(Path);
		if (Directory.IsEmpty())
		{
			Directory = Root;
		}

		NxFr::GUID DirectoryId = PathToId(Directory);
		return Map[DirectoryId];
	}

	AssetsBrowserItem* AssetsBrowserPanel::GetIterator(AssetsBrowserItem* Item)
	{
		if (Item->Child)
		{
			return Item->Child;
		}

		if (Item->Next)
		{
			return Item->Next;
		}

		AssetsBrowserItem* Parent = Item->Parent;
		while (Parent && !Parent->Next)
		{
			Parent = Parent->Parent;
		}

		if (Parent)
		{
			return Parent->Next;
		}

		return nullptr;
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
				Item = GetIterator(Item);
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

			Item = GetIterator(Item);
		}
	}

	void AssetsBrowserPanel::Select(AssetsBrowserItem* Item, bool Additive, bool List)
	{
		if (!Item || !Additive)
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
			NxFr::StringView P1 = Item->Path;
			NxFr::StringView P2 = Selected->Path;
			AssetsBrowserItem* I1 = P1 < P2 ? Item : Selected;
			AssetsBrowserItem* I2 = P1 >= P2 ? Item : Selected;
			while (I1 && I1 != I2)
			{
				if (IsVisible(I1))
				{
					I1->Selected = Item->Selected;
					Selection.Append(I1);
				}

				I1 = GetIterator(I1);
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

	NxFr::GUID AssetsBrowserPanel::PathToId(NxFr::StringView Path)
	{
		return NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::AssetExtension) ?
			Assets->PathToId(NxFr::Path::GetPathWithoutExtension(Path)) :
			NxFr::Hash<>::HashObject(Path);
	}

	NxFr::String AssetsBrowserPanel::FileToPath(NxFr::StringView Path)
	{
		if (Path == NxFr::Paths::Assets)
		{
			return Root;
		}

		return NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::PathToFile(NxFr::StringView Path)
	{
		if (Path == Root)
		{
			return NxFr::Paths::Assets.Data;
		}

		return NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::ConvertPath(NxFr::StringView Path)
	{
		return NxFr::Path::ChangeExtension(Path, NxEn::AssetMetadata::AssetExtension);
	}
}
