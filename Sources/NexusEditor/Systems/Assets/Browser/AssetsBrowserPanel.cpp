#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionCreate.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionRename.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDelete.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionLoad.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionImport.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReimport.h"

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
	const static NxEn::Command CmdAssetCreateDirectory = NxEn::Command::Create("Assets.Create.Directory"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Create(Path, 0);
	}));
	const static NxEn::Command CmdAssetCreateFile = NxEn::Command::Create("Assets.Create.File"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Type)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Create(Path, NxFr::StringId(Type));
	}));
	const static NxEn::Command CmdAssetMove = NxEn::Command::Create("Assets.Move"_Sid, "Move path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Move(Path, Target);
	}));
	const static NxEn::Command CmdAssetDuplicate = NxEn::Command::Create("Assets.Duplicate"_Sid, "Duplicate path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Duplicate(Path, Target);
	}));
	const static NxEn::Command CmdAssetDelete = NxEn::Command::Create("Assets.Delete"_Sid, "Delete path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Delete(Path);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	AssetsBrowserPanel::AssetsBrowserPanel()
		: Style(), Assets(nullptr),Inputs(nullptr), Items(), Map(), Selection(), Selected(nullptr), Filter(64)
	{
		Actions.Append(new AssetsBrowserActionCreate());
		Actions.Append(new AssetsBrowserActionDuplicate());
		Actions.Append(new AssetsBrowserActionRename());
		Actions.Append(new AssetsBrowserActionMove());
		Actions.Append(new AssetsBrowserActionDelete());
		Actions.Append(new AssetsBrowserActionLoad());
		Actions.Append(new AssetsBrowserActionImport());
		Actions.Append(new AssetsBrowserActionReimport());
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
		NEXUS_ASSERT(Path != Root, Default, "Can't create the Assets/ folder");

		NxFr::String AssetPath = PathToAsset(Path);

		AssetsBrowserItem* Item = AppendItem(AssetPath);
		Item->Create(Path, Type);
		AttachItem(Item, GetParent(AssetPath), true);
	}

	void AssetsBrowserPanel::Move(NxFr::StringView Path, NxFr::StringView Target)
	{
		NEXUS_ASSERT(Path != Root, Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = PathToId(Path);

		AssetsBrowserItem* Item = Map[Id];
		DetachItem(Item, true);
		Item->Move(Target);
		AttachItem(Item, GetParent(Item->Path), true);
	}

	void AssetsBrowserPanel::Duplicate(NxFr::StringView Path, NxFr::StringView Target)
	{
		NEXUS_ASSERT(Path != Root, Default, "Can't duplicate the Assets/ folder");

		NxFr::GUID Id = PathToId(Path);

		AssetsBrowserItem* Item = Map[Id];
		Item = DuplicateItem(Item, nullptr);
		Item->Duplicate(Target);
		AttachItem(Item, GetParent(Item->Path), true);
	}

	void AssetsBrowserPanel::Delete(NxFr::StringView Path)
	{
		NEXUS_ASSERT(Path != Root, Default, "Can't delete the Assets/ folder");

		NxFr::GUID Id = PathToId(Path);

		AssetsBrowserItem* Item = Map[Id];
		Item->Delete();
		DetachItem(Item, true);
		RemoveItem(Item);
	}

	bool AssetsBrowserPanel::Exist(NxFr::StringView Path)
	{
		return NxFr::Path::Exist(PathToDisk(Path));
	}

	NxFr::String AssetsBrowserPanel::ValidatePath(NxFr::String Path)
	{
		bool Directory = NxFr::Path::IsDirectory(Path);
		NxFr::StringView Name = Directory ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path);
		NxFr::StringView Extension = NxFr::Path::GetExtension(Path);

		uint64 Count = 1;
		NxFr::String Target;
		while (Exist(Path))
		{
			Target = Name + " " + NxFr::StringUtility::ToString(Count);
			if (Directory)
			{
				NxFr::Path::ChangeDirectoryName(Path, Target);
			}
			else
			{
				Target += "." + Extension;
				NxFr::Path::ChangeFileName(Path, Target);
			}

			Count++;
		}

		return Path;
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

		ProcessAction();
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
				if (Inputs->CheckButton(NxEn::Input::Button::MouseRight, NxEn::Input::State::Pressed))
				{
					OpenContext(Item);
				}
			}

			DrawContext(Item);
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

	void AssetsBrowserPanel::DrawContext(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		if (ImGui::BeginPopup(Item->ImGuiText.C()))
		{
			for (auto Action : Actions)
			{
				if (ImGui::MenuItem(Action->GetLabel().C()))
				{
					ActionRequested = Action;
					break;
				}
			}

			ImGui::EndPopup();
		}
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

	void AssetsBrowserPanel::OpenContext(AssetsBrowserItem* Item)
	{
		ImGui::OpenPopup(Item->ImGuiText.C());
	}

	void AssetsBrowserPanel::Clear()
	{
		Selection.Clear();
		Selected = nullptr;
		Filtered.Clear();
		Filter.Clear();
		ActionRequested = nullptr;

		RemoveItem(Items);
		Map.Clear();
		Items = nullptr;
	}

	void AssetsBrowserPanel::Fetch()
	{
		Items = FetchItems(NxFr::Paths::Assets, nullptr);
		Items->ImGuiText = RootImGui;
		Items->Expanded = true;

		PurgeDuplicates(Items);
	}

	AssetsBrowserItem* AssetsBrowserPanel::FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent)
	{
		NxFr::String ItemPath = DiskToPath(Path);
		AssetsBrowserItem* Item = AppendItem(ItemPath);
		UpdateItem(Item, ItemPath, true, false);

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
				RemoveItem(Item->Next);
			}
			else if (Item->Previous && Item->GetPrettyName() == Item->Previous->GetPrettyName())
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

	AssetsBrowserItem* AssetsBrowserPanel::AppendItem(NxFr::StringView Path)
	{
		AssetsBrowserItem* Item = nullptr;
		if (NxFr::Path::IsDirectory(Path) || Path == Root)
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

		return Item;
	}

	void AssetsBrowserPanel::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool AddId, bool RemoveId)
	{
		if (RemoveId)
		{
			if (Map.ContainsKey(Item->Id))
			{
				Map.Remove(Item->Id);
			}
		}

		Item->Id = PathToId(Path);
		Item->Path = Path;
		Item->ImGuiText = Item->GetPrefix() + " " + Item->GetPrettyName() + "##" + NxFr::StringUtility::ToString(Item->Id);

		if (AddId)
		{
			Map.Append(Item->Id, Item);
		}
	}

	AssetsBrowserItem* AssetsBrowserPanel::DuplicateItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent)
	{
		if (!Item)
		{
			return nullptr;
		}

		AssetsBrowserItem* Copy = AppendItem(Item->Path);
		UpdateItem(Copy, Item->Path, false, false);

		Copy->Parent = Parent;
		if (Item->Child)
		{
			Copy->Child = DuplicateItem(Item->Child, Copy);
		}
		if (Item->Next && Parent)
		{
			Copy->Next = DuplicateItem(Item->Next, Parent);
			Copy->Next->Previous = Copy;
		}

		return Copy;
	}

	void AssetsBrowserPanel::RemoveItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		DetachItem(Item, false);
		while (Item->Child)
		{
			RemoveItem(Item->Child);
		}

		if (Item == Selected)
		{
			Selected = nullptr;
		}
		if (Selection.Contains(Item))
		{
			Selection.Remove(Item);
		}
		if (Filtered.Contains(Item))
		{
			Filtered.Remove(Item);
		}

		Map.Remove(Item->Id);
		delete Item;
	}

	void AssetsBrowserPanel::AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent, bool Sort)
	{
		if (!Parent)
		{
			return;
		}

		Item->Parent = Parent;

		Item->Next = Parent->Child;
		if (Parent->Child)
		{
			Parent->Child->Previous = Item;
		}
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

	void AssetsBrowserPanel::ProcessAction()
	{
		if (!ActionRequested)
		{
			return;
		}

		NxFr::Array<AssetsBrowserItem*> ActionItems = GatherActionItems(ActionRequested);
		ActionRequested->Execute(ActionItems);

		ActionRequested = nullptr;
	}

	NxFr::Array<AssetsBrowserItem*> AssetsBrowserPanel::GatherActionItems(AssetsBrowserAction* Action)
	{
		NxFr::Set<AssetsBrowserItem*> Result = Selection;

		if (Action->IsRecursive())
		{
			for (auto S : Selection)
			{
				GatherChildren(S->Child, Result);
			}
		}

		return NxFr::ContainersUtils::ToArray<AssetsBrowserItem*>(Result);
	}

	void AssetsBrowserPanel::GatherChildren(AssetsBrowserItem* Item, NxFr::Set<AssetsBrowserItem*>& Result)
	{
		if (Item == nullptr)
		{
			return;
		}

		Result.Append(Item);

		GatherChildren(Item->Child, Result);

		while (Item)
		{
			GatherChildren(Item->Next, Result);
			Item = Item->Next;
		}
	}

	NxFr::GUID AssetsBrowserPanel::PathToId(NxFr::StringView Path)
	{
		if (Path == Root)
		{
			return NxFr::Hash<>::HashObject(RootImGui);
		}

		return NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::AssetExtension) ?
			Assets->PathToId(NxFr::Path::GetPathWithoutExtension(Path)) :
			NxFr::Hash<>::HashObject(Path);
	}

	NxFr::String AssetsBrowserPanel::DiskToPath(NxFr::StringView Path)
	{
		if (Path == NxFr::Paths::Assets)
		{
			return Root;
		}

		return NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::PathToDisk(NxFr::StringView Path)
	{
		if (Path == Root)
		{
			return NxFr::Paths::Assets.Data;
		}

		return NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::PathToAsset(NxFr::StringView Path)
	{
		return NxFr::Path::ChangeExtension(Path, NxEn::AssetMetadata::AssetExtension);
	}
}
