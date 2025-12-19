#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionCreate.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDuplicate.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionRename.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionMove.h"
#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDelete.h"

namespace NxEd
{
	static HierarchyPanel* Panel = NxEn::GUI::Panel::Create<HierarchyPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/World/Hierarchy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel HierarchyPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyPanel)

	void HierarchyPanel::Refresh()
	{
		WorldsIds = Worlds->GetWorlds();
		Labels = NxFr::Array<NxFr::StringView>(WorldsIds.GetCount());

		for (uint64 Index = 0; Index < WorldsIds.GetCount(); ++Index)
		{
			Labels[Index] = WorldsIds[Index];
		}

		Menu.Remove("Worlds/");
		Menu.AddMenuEnum("Worlds", &WorldIndex, Labels, [&]() { SelectWorld(WorldsIds[WorldIndex]); }, 1);

		SelectWorld(NxEn::WorldSystem::MainId);
	}

	void HierarchyPanel::SelectWorld(NxFr::StringId Id)
	{
		WorldIndex = WorldsIds.Find(Id).Id();

		ClearItems();
		FetchItems();
	}

	void HierarchyPanel::SelectGameObject(NxEn::GameObject* Target)
	{
		if (!GetWorld()->Belong(Target))
		{
			return;
		}

		Select(Target, false, false);
	}

	void HierarchyPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetTitle("Hierarchy");
		SetGuiFlag(ImGuiWindowFlags_MenuBar);

		Menu.AddMenuItem("Refresh", { this, &HierarchyPanel::Refresh }, 0);

		Actions.Append(new HierarchyActionCreate());
		Actions.Append(new HierarchyActionDuplicate());
		Actions.Append(new HierarchyActionRename());
		Actions.Append(new HierarchyActionMove());
		Actions.Append(new HierarchyActionDelete());
	}

	void HierarchyPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void HierarchyPanel::OnEnable()
	{
		Panel::OnEnable();

		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Inputs = NxEn::Application::GetSystem<NxEn::InputSystem>();

		Worlds->GetOnGameObjectEvent() += { this, &HierarchyPanel::OnGameObjectChanged };

		Style.Reset();
		Style.Width = -1.0f;
		Style.WidthLabel = 0.0f;

		Refresh();
	}

	void HierarchyPanel::OnDisable()
	{
		ClearItems();

		Worlds->GetOnGameObjectEvent() -= { this, &HierarchyPanel::OnGameObjectChanged };

		Panel::OnDisable();
	}

	void HierarchyPanel::OnGui(float TimeStep)
	{
		DrawHeader(TimeStep);
		DrawItem(GetWorld()->GetRoot());

		ProcessAction();
	}

	void HierarchyPanel::DrawHeader(float TimeStep)
	{
		Menu.Tick(TimeStep);

		if (ImGui::BeginMenuBar())
		{
			if (NxEn::GUI::Drawer<NxFr::String>::Field(Filter, "Filter", "", &Style))
			{
				Find();
			}
		}
		ImGui::EndMenuBar();
	}

	void HierarchyPanel::DrawItem(NxEn::GameObject* Instance)
	{
		if (!Instance)
		{
			return;
		}

		HierarchyItem& Item = Items[Instance];
		bool Browse = Filter.IsEmpty();
		bool ExpandChanged = false;

		if (Browse || Filtered.Contains(Instance))
		{
			// Draw
			uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
				| (!Item.GameObject->GetChild() ? ImGuiTreeNodeFlags_Leaf : 0)
				| (Item.Selected ? ImGuiTreeNodeFlags_Selected : 0);

			if (Browse)
			{
				ImGui::SetNextItemOpen(Item.Expanded, ImGuiCond_Always);
				bool State = ImGui::TreeNodeEx(Item.GetImGuiText().C(), Flag);
				ExpandChanged = Item.Expanded != State;
				Item.Expanded = State;
			}
			else
			{
				ImGui::Selectable(Item.GetImGuiText().C(), Item.Selected);
			}

			// Inputs
			if (ImGui::IsItemHovered())
			{
				if (Inputs->CheckButton(NxEn::Input::Button::MouseLeft, NxEn::Input::State::Pressed) && !ExpandChanged)
				{
					SelectItem(Instance);
				}
				if (Inputs->CheckButton(NxEn::Input::Button::MouseRight, NxEn::Input::State::Pressed))
				{
					OpenContext(Instance);
				}
			}

			DrawContext(Instance);
		}

		// Iterate
		if (Item.Expanded)
		{
			if (Browse)
			{
				ImGui::TreePush(Item.GetImGuiText().C());
			}

			DrawItem(Item.GameObject->GetChild());

			if (Browse)
			{
				ImGui::TreePop();
			}
		}

		DrawItem(Item.GameObject->GetNext());
	}

	void HierarchyPanel::DrawContext(NxEn::GameObject* Instance)
	{
		if (!Instance)
		{
			return;
		}

		if (ImGui::BeginPopup(Items[Instance].GetImGuiText().C()))
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

	void HierarchyPanel::SelectItem(NxEn::GameObject* Instance)
	{
		if (Inputs->CheckModifier(NxEn::Input::Modifier::Ctrl))
		{
			Select(Instance, true);
		}
		else if (Inputs->CheckModifier(NxEn::Input::Modifier::Shift))
		{
			Select(Instance, true, true);
		}
		else
		{
			Select(Instance);
		}
	}

	void HierarchyPanel::OpenContext(NxEn::GameObject* Instance)
	{
		ImGui::OpenPopup(Items[Instance].GetImGuiText().C());
	}

	void HierarchyPanel::ClearItems()
	{
		Selection.Clear();
		Selected = nullptr;
		Filtered.Clear();
		Filter.Clear();

		Items.Clear();
	}

	void HierarchyPanel::FetchItems()
	{
		NxEn::World* World = GetWorld();
		Items.Reserve(World->GetObjectCount());

		for (auto& Instance : World->GetObjects())
		{
			AddItem(&Instance);
		}
	}

	void HierarchyPanel::AddItem(NxEn::GameObject* Instance)
	{
		Items.Append(Instance, Instance);
	}

	void HierarchyPanel::RemoveItem(NxEn::GameObject* Instance)
	{
		Items.Remove(Instance);
		if (Selection.Contains(Instance))
		{
			Selection.Remove(Instance);
		}
		if (Filtered.Contains(Instance))
		{
			Filtered.Remove(Instance);
		}
	}

	void HierarchyPanel::OnGameObjectChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId)
	{
		NxEn::World* World = GetWorld();
		if (WorldId != World->GetId())
		{
			return;
		}

		NxEn::GameObject* GameObject = World->GetGameObject(GameObjectId);
		if  (EventId == NxEn::WorldSystem::AppendedId)
		{
			AddItem(GameObject);
		}
		else if (EventId == NxEn::WorldSystem::RemovedId)
		{
			RemoveItem(GameObject);
		}
	}

	void HierarchyPanel::Find()
	{
		Filtered.Clear();
		if (Filter.IsEmpty())
		{
			return;
		}

		NxFr::Array<NxEn::GameObject*> Result = GetWorld()->Find(Filter);
		for (auto Instance : Result)
		{
			Show(Instance);
			Filtered.Append(Instance);
		}
	}

	void HierarchyPanel::Select(NxEn::GameObject* Instance, bool Additive, bool List)
	{
		if (!Instance || !Additive)
		{
			for (auto& S : Selection)
			{
				Items[S].Selected = false;
			}

			Selection.Clear();
			Selected = nullptr;
		}

		if (!Instance)
		{
			return;
		}

		HierarchyItem& Item = Items[Instance];
		Item.Selected = !Item.Selected;
		Show(Instance);

		if (List)
		{
			uint64 P1 = Instance->GetOrderIndex();
			uint64 P2 = Selected->GetOrderIndex();
			NxEn::GameObject* I1 = P1 < P2 ? Instance : Selected;
			NxEn::GameObject* I2 = P1 >= P2 ? Instance : Selected;
			while (I1 && I1 != I2)
			{
				if (IsVisible(I1))
				{
					Items[I1].Selected = Item.Selected;
					Selection.Append(I1);
				}

				I1 = I1->GetIterator();
			}
			if (I1)
			{
				if (IsVisible(I1))
				{
					Items[I1].Selected = Item.Selected;
					Selection.Append(I1);
				}
			}
		}

		Selected = Instance;
		Selection.Append(Selected);
	}

	void HierarchyPanel::Show(NxEn::GameObject* Instance)
	{
		NxEn::GameObject* Parent = Instance->GetParent();
		while (Parent)
		{
			Items[Parent].Expanded = true;
			Parent = Parent->GetParent();
		}
	}

	bool HierarchyPanel::IsVisible(NxEn::GameObject* Instance)
	{
		bool Visible = true;
		NxEn::GameObject* Parent = Instance->GetParent();

		while (Parent)
		{
			Visible &= Items[Parent].Expanded;
			Parent = Parent->GetParent();
		}

		return Visible;
	}

	void HierarchyPanel::ProcessAction()
	{
		if (!ActionRequested)
		{
			return;
		}

		NxFr::Array<NxEn::GameObject*> ActionItems = GatherActionItems(ActionRequested);
		ActionRequested->Execute(ActionItems);

		ActionRequested = nullptr;
	}

	NxFr::Array<NxEn::GameObject*> HierarchyPanel::GatherActionItems(HierarchyAction* Action)
	{
		NxFr::Set<NxEn::GameObject*> Result = Selection;

		if (Action->IsRecursive())
		{
			for (auto S : Selection)
			{
				GatherChildren(S->GetChild(), Result);
			}
		}

		if (Action->IsLastSelectedFirst())
		{
			Result.Remove(Selected);
		}

		NxFr::List<NxEn::GameObject*> Items;
		Items.AppendRange(Result);
		Items.Sort([](NxEn::GameObject* A, NxEn::GameObject* B) { return A->GetOrderIndex() < B->GetOrderIndex(); });
		if (Action->IsLastSelectedFirst())
		{
			Items.Insert(0, Selected);
		}

		return NxFr::ContainersUtils::ToArray<NxEn::GameObject*>(Items);
	}

	void HierarchyPanel::GatherChildren(NxEn::GameObject* Item, NxFr::Set<NxEn::GameObject*>& Result)
	{
		if (Item == nullptr)
		{
			return;
		}

		Result.Append(Item);

		GatherChildren(Item->GetChild(), Result);

		while (Item)
		{
			GatherChildren(Item->GetNext(), Result);
			Item = Item->GetNext();
		}
	}

	void HierarchyPanel::SortActions()
	{
		Actions.Sort([](HierarchyAction* A, HierarchyAction* B) { return *A <= *B; });
	}
}
