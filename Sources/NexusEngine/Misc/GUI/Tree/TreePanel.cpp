#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/Tree/TreePanel.h"

#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	TreePanel::TreePanel()
		: Menu(),
		Inputs(nullptr),
		Root(nullptr),
		Selection(), Selected(nullptr),
		Filtered(), Filter(256),
		Actions(), ActionRequested(nullptr)
	{
	}

	TreePanel::~TreePanel()
	{
	}

	void TreePanel::Clear()
	{
		Selection.Clear();
		Selected = nullptr;
		Filtered.Clear();
		Filter.Clear();
		ActionRequested = nullptr;

		Root = nullptr;
	}

	void TreePanel::Refresh()
	{
		Clear();
		Root = FetchRootItem();
	}

	void TreePanel::Find(NxFr::StringView Query)
	{
		Filter.Clear();
		Filter += Query;

		FindItem();
	}

	void TreePanel::Select(TreeItem* Item)
	{
		SelectItem(Item, true, false, false);
	}

	void TreePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		Menu.AddMenuItem("Refresh", { this, &TreePanel::Refresh });
	}

	void TreePanel::OnShutdown()
	{
		for (uint64 Index = 0; Index < Actions.GetCount(); ++Index)
		{
			delete Actions[Index];
		}

		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void TreePanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.SetEnabled(true);

		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);

		Inputs = Application::GetSystem<InputSystem>();
	}

	void TreePanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void TreePanel::OnDraw()
	{
		DrawHeader();
		DrawItem(Root);

		ProcessAction();
	}

	void TreePanel::OnCreateItem(TreeItem* Item)
	{
		
	}

	void TreePanel::OnDestroyItem(TreeItem* Item)
	{
		if (Root == Item)
		{
			Root = nullptr;
		}
		if (Selected == Item)
		{
			Selected = nullptr;
		}
		if (Selection.TryGet(Item))
		{
			Selection.Remove(Item);
		}
		if (Filtered.TryGet(Item))
		{
			Filtered.Remove(Item);
		}
	}

	void TreePanel::OnSelectItem(TreeItem* Item, bool State)
	{
	}

	void TreePanel::DrawHeader()
	{
		GUI::Transform Visual = GUI::Transform(-NxFr::Vector2f::One, -NxFr::Vector2f::One, -1.0f);
		Menu.Draw();

		if (ImGui::BeginMenuBar())
		{
			if (NxEn::GUI::Drawer<NxFr::String>::Field(Filter, "Filter:", "", Visual))
			{
				FindItem();
			}
		}
		ImGui::EndMenuBar();

		NxEn::GUI::Drawer<NxFr::String>::Property(Selected ? Selected->GetDescription() : "", "Selected", Visual);

		ImGui::Separator();
	}

	void TreePanel::DrawItem(TreeItem* Item)
	{
		if (!Item)
		{
			return;
		}

		Item->Draw();

		bool Browse = Filter.IsEmpty();
		bool ExpandChanged = false;

		if (Browse || Filtered.TryGet(Item))
		{
			// Draw
			uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
				| (Item->IsLeaf() ? ImGuiTreeNodeFlags_Leaf : 0)
				| (Item->IsSelected() ? ImGuiTreeNodeFlags_Selected : 0);

			if (Browse)
			{
				ImGui::SetNextItemOpen(Item->IsOpened(), ImGuiCond_Always);
				bool State = ImGui::TreeNodeEx(Item->GetName().C(), Flag);
				ExpandChanged = Item->IsOpened() != State;
				Item->Open(State);
			}
			else
			{
				ImGui::Selectable(Item->GetName().C(), Item->IsSelected());
			}

			// Inputs
			if (ImGui::IsItemHovered())
			{
				if (Inputs->CheckButton(NxEn::Input::Button::MouseLeft, NxEn::Input::State::Pressed) && !ExpandChanged)
				{
					HandleSelection(Item);
				}
				if (Inputs->CheckButton(NxEn::Input::Button::MouseRight, NxEn::Input::State::Pressed))
				{
					OpenContext(Item);
				}
			}

			DrawContext(Item);
		}

		// Iterate
		if (Item->IsOpened())
		{
			if (Browse)
			{
				ImGui::TreePush(Item->GetName().C());
			}

			DrawItem(Item->GetChild());

			if (Browse)
			{
				ImGui::TreePop();
			}
		}

		DrawItem(Item->GetNext());
	}

	void TreePanel::DrawContext(TreeItem* Item)
	{
		if (!Item)
		{
			return;
		}

		if (ImGui::BeginPopup(Item->GetName().C()))
		{
			for (auto Action : Actions)
			{
				if (ImGui::MenuItem(Action->GetName().C()))
				{
					ActionRequested = Action;
					break;
				}
			}

			ImGui::EndPopup();
		}
	}

	void TreePanel::OpenContext(TreeItem* Item)
	{
		ImGui::OpenPopup(Item->GetName().C());
	}

	void TreePanel::HandleSelection(TreeItem* Item)
	{
		if (Inputs->CheckModifier(NxEn::Input::Modifier::Ctrl))
		{
			SelectItem(Item, !Item->IsSelected(), true, false);
		}
		else if (Inputs->CheckModifier(NxEn::Input::Modifier::Shift))
		{
			SelectItem(Item, !Item->IsSelected(), true, true);
		}
		else
		{
			SelectItem(Item, true, false, false);
		}
	}

	void TreePanel::FindItem()
	{
		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}

		NxEn::Utils::Filter F(Filter);
		TreeItem* Item = Root;
		while (Item)
		{
			if (F.FilterInstance(Item->GetItemName(), Item->GetItemId(), Item->GetItemType()))
			{
				ShowItem(Item);
				Filtered.Append(Item);
			}

			Item = Item->GetIterator();
		}
	}

	void TreePanel::SelectItem(TreeItem* Item, bool State, bool Additive, bool List)
	{
		auto ApplyState = [&](TreeItem* Instance)
		{
			if (State)
			{
				ShowItem(Instance);
			}

			Instance->Select(State);
			OnSelectItem(Instance, State);

			if (State && !Selection.TryGet(Instance))
			{
				Selection.Append(Instance);
			}
			else if (!State && Selection.TryGet(Instance))
			{
				Selection.Remove(Instance);
			}
		};

		if (!Item || !Additive)
		{
			for (auto& Item : Selection)
			{
				Item->Select(false);
				OnSelectItem(Item, false);
			}

			Selection.Clear();
			Selected = nullptr;
		}

		if (!Item || Item->IsSelected() == State)
		{
			return;
		}

		ApplyState(Item);

		if (List && Selected)
		{
			bool Compare = *Item < *Selected;
			TreeItem* I1 = Compare ? Item : Selected;
			TreeItem* I2 = Compare ? Selected : Item;

			while (I1 && I1 != I2)
			{
				if (IsItemVisible(I1))
				{
					ApplyState(I1);
				}

				I1 = I1->GetIterator();
			}
			if (I1)
			{
				if (IsItemVisible(I1))
				{
					ApplyState(I1);
				}
			}
		}

		if (State && Selected != Item)
		{
			Selected = Item;
		}
		else if (!State && Selected == Item)
		{
			Selected = nullptr;
		}
	}

	void TreePanel::ShowItem(TreeItem* Item)
	{
		TreeItem* Parent = Item->GetParent();
		while (Parent)
		{
			Parent->Open(true);
			Parent = Parent->GetParent();
		}
	}

	bool TreePanel::IsItemVisible(TreeItem* Item)
	{
		bool Visible = true;
		TreeItem* Parent = Item->GetParent();

		while (Parent)
		{
			Visible &= Parent->IsOpened();
			Parent = Parent->GetParent();
		}

		return Visible;
	}

	void TreePanel::ProcessAction()
	{
		if (!ActionRequested)
		{
			return;
		}

		NxFr::Array<TreeItem*> ActionItems = GatherActionItems(ActionRequested);
		ActionRequested->Execute(ActionItems);

		ActionRequested = nullptr;
	}
	
	NxFr::Array<TreeItem*> TreePanel::GatherActionItems(TreeAction* Action)
	{
		NxFr::Set<TreeItem*> Result;
		for (auto S : Selection)
		{
			GatherActionItems(S, Result, Action->IsRecursive());
		}

		if (Action->IsLastSelectedFirst())
		{
			Result.Remove(Selected);
		}

		NxFr::List<TreeItem*> Items;
		Items.AppendRange(Result);
		NxFr::ContainerUtility::Sort<TreeItem*>(Items, [](TreeItem* A, TreeItem* B) { return *A < *B; });

		if (Action->IsLastSelectedFirst())
		{
			Items.Insert(0, Selected);
		}

		return NxFr::ContainerUtility::ToArray<TreeItem*>(Items);
	}

	void TreePanel::GatherActionItems(TreeItem* Item, NxFr::Set<TreeItem*>& Result, bool Recursive)
	{
		if (!Item)
		{
			return;
		}

		Result.Append(Item);

		if (Recursive)
		{
			TreeItem* Iterator = Item->GetChild();
			while (Iterator)
			{
				GatherActionItems(Iterator, Result, Recursive);
				Iterator = Iterator->GetNext();
			}
		}
	}

	void TreePanel::SortActions()
	{
		NxFr::ContainerUtility::Sort<TreeAction*>(Actions, [](TreeAction* A, TreeAction* B) { return A->GetPriority() <= B->GetPriority(); });
	}
}
