#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(TreePanel)

	TreePanel::TreePanel()
		: Menu(), Style(),
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
		Root = FetchItems();
	}

	void TreePanel::Find(NxFr::StringView Query)
	{
		Filter.Clear();
		Filter += Query;

		Find();
	}

	void TreePanel::Select(TreeItem* Item)
	{
		Select(Item, false, false);
	}

	void TreePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();
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

		SetGuiFlag(ImGuiWindowFlags_MenuBar);

		Inputs = Application::GetSystem<InputSystem>();

		Style.Reset();
		Style.Width = -1.0f;
		Style.WidthLabel = -1.0f;

		Refresh();
	}

	void TreePanel::OnDisable()
	{
		Clear();

		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void TreePanel::OnGui(float TimeStep)
	{
		DrawHeader(TimeStep);
		DrawItem(TimeStep, Root);

		ProcessAction();
	}

	void TreePanel::ClearItem(TreeItem* Item)
	{
		if (Selected == Item)
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
	}

	void TreePanel::DrawHeader(float TimeStep)
	{
		if (ImGui::BeginMenuBar())
		{
			Style.Width = GUI::Style::GetVar(GUI::Style::IdWidthButton);
			if (NxEn::GUI::Draw::Button("Refresh", &Style))
			{
				Refresh();
			}
		}
		ImGui::EndMenuBar();

		Menu.Tick(TimeStep);

		if (ImGui::BeginMenuBar())
		{
			Style.Width = -1.0f;
			Style.WidthLabel = 0.0f;
			if (NxEn::GUI::Drawer<NxFr::String>::Field(Filter, "Filter", "", &Style))
			{
				Find();
			}
		}
		ImGui::EndMenuBar();

		NxEn::GUI::Drawer<NxFr::String>::Property(Selected ? Selected->GetDescription() : "", "Selected", &Style);

		ImGui::Separator();
	}

	void TreePanel::DrawItem(float TimeStep, TreeItem* Item)
	{
		if (!Item)
		{
			return;
		}

		Item->Tick(TimeStep);

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
				ImGui::SetNextItemOpen(Item->IsOpened(), ImGuiCond_Always);
				bool State = ImGui::TreeNodeEx(Item->GetLabel().C(), Flag);
				ExpandChanged = Item->IsOpened() != State;
				Item->Open(State);
			}
			else
			{
				ImGui::Selectable(Item->GetLabel().C(), Item->IsSelected());
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
		if (Item->IsOpened())
		{
			if (Browse)
			{
				ImGui::TreePush(Item->GetLabel().C());
			}

			DrawItem(TimeStep, Item->GetChild());

			if (Browse)
			{
				ImGui::TreePop();
			}
		}

		DrawItem(TimeStep, Item->GetNext());
	}

	void TreePanel::DrawContext(TreeItem* Item)
	{
		if (!Item)
		{
			return;
		}

		if (ImGui::BeginPopup(Item->GetLabel().C()))
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

	void TreePanel::OpenContext(TreeItem* Item)
	{
		ImGui::OpenPopup(Item->GetLabel().C());
	}

	void TreePanel::SelectItem(TreeItem* Item)
	{
		if (Inputs->CheckModifier(NxEn::Input::Modifier::Ctrl))
		{
			Select(Item, true, false);
		}
		else if (Inputs->CheckModifier(NxEn::Input::Modifier::Shift))
		{
			Select(Item, true, true);
		}
		else
		{
			Select(Item, false, false);
		}
	}

	void TreePanel::Find()
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
			if (F.FilterObject(Item->GetDescription(), Item->GetId(), Item->GetType()))
			{
				Show(Item);
				Filtered.Append(Item);
			}

			Item = Item->GetIterator();
		}
	}

	void TreePanel::Select(TreeItem* Item, bool Additive, bool List)
	{
		if (!Item || !Additive)
		{
			for (auto& Item : Selection)
			{
				Item->Select(false);
			}

			Selection.Clear();
			Selected = nullptr;
		}

		if (!Item)
		{
			return;
		}

		Item->Select(!Item->IsSelected());
		Show(Item);

		if (List && Selected)
		{
			bool Compare = *Item < *Selected;
			TreeItem* I1 = Compare ? Item : Selected;
			TreeItem* I2 = Compare ? Selected : Item;

			while (I1 && I1 != I2)
			{
				if (IsVisible(I1))
				{
					I1->Select(Item->IsSelected());
					Selection.Append(I1);
				}

				I1 = I1->GetIterator();
			}
			if (I1)
			{
				if (IsVisible(I1))
				{
					I1->Select(Item->IsSelected());
					Selection.Append(I1);
				}
			}
		}

		Selected = Item;
		Selection.Append(Selected);
	}

	void TreePanel::Show(TreeItem* Item)
	{
		TreeItem* Parent = Item->GetParent();
		while (Parent)
		{
			Parent->Open(true);
			Parent = Parent->GetParent();
		}
	}

	bool TreePanel::IsVisible(TreeItem* Item)
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
		Items.Sort([](TreeItem* A, TreeItem* B) { return *A < *B; });

		if (Action->IsLastSelectedFirst())
		{
			Items.Insert(0, Selected);
		}

		return NxFr::ContainersUtils::ToArray<TreeItem*>(Items);
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
			GatherActionItems(Item->GetChild(), Result, Recursive);
		}
	}

	void TreePanel::SortActions()
	{
		Actions.Sort([](TreeAction* A, TreeAction* B) { return *A <= *B; });
	}
}
