#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"

#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	namespace Rework
	{
		NxFr::Array<NxFr::GUID> TreePanel::GetSelected(NxFr::GUID* Active)
		{
			if (Active)
			{
				*Active = Selected;
			}

			return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Selection);
		}

		NxFr::GUID TreePanel::GetRoot()
		{
			return Root;
		}

		void TreePanel::SetRoot(NxFr::GUID Id)
		{
			Root = Id;
		}

		void TreePanel::Clear()
		{
			Root = 0;

			Selection.Clear();
			Selected = 0;
			Filtered.Clear();
			Filter = "";
			Action = "";
		}

		void TreePanel::Refresh()
		{

		}

		void TreePanel::Select(NxFr::GUID Id, bool State, bool Additive, bool List, bool ApplyFilter)
		{
			auto ApplyState = [&](NxFr::GUID ItemId, bool ItemState, bool Force)
			{
				if (!Force && !IsVisible(ItemId))
				{
					return;
				}

				if (!Force && (ApplyFilter && IsFiltered(ItemId)))
				{
					return;
				}

				if (ItemState)
				{
					SetVisible(ItemId);
					Selection.TryAppend(ItemId);
				}
				else
				{
					Selection.TryRemove(ItemId);
				}

				OnSelectItem(ItemId, ItemState);
			};

			if (State && !Additive)
			{
				for (auto& Item : Selection)
				{
					ApplyState(Item, false, true);
				}

				Selection.Clear();
				Selected = 0;
			}

			TreeItem* Instance = GetItem(Id);
			if (!Instance || IsSelected(Id) == State)
			{
				return;
			}

			ApplyState(Id, State, false);

			if (List && Selected)
			{
				bool Compare = Instance->Compare(*GetItem(Selected));
				NxFr::GUID Current = Compare ? Id : Selected;
				NxFr::GUID Target = Compare ? Selected : Id;

				do
				{
					ApplyState(Current, State, false);
					Current = GetIterator(Current);
				} while (Current != Target);
			}

			if (State && Selected != Id)
			{
				Selected = Id;
			}
			else if (!State && Selected == Id)
			{
				Id = 0;
			}
		}

		bool TreePanel::IsSelected(NxFr::GUID Id, bool* Active)
		{
			TreeItem* Item = GetItem(Id);
			if (!Item)
			{
				return false;
			}

			if (Active)
			{
				*Active = Selected == Id;
			}

			return Selection.TryGet(Id);
		}

		void TreePanel::Find(NxFr::StringView Query)
		{
			Filter.Clear();
			Filter += Query;

			Filtered.Clear();
			if (Filter.IsEmpty())
			{
				return;
			}

			NxEn::Utils::Filter F(Filter);
			TreeItem* Item = GetItem(Root);
			while (Item)
			{
				if (F.FilterInstance(Item->GetName(), Item->GetId(), Item->GetType()))
				{
					SetVisible(Item->GetId());
					Filtered.Append(Item->GetId());
				}

				Item = GetItem(GetIterator(Item->GetId()));
			}
		}

		bool TreePanel::IsFiltered(NxFr::GUID Id)
		{
			return !Filter.IsEmpty() && Filtered.TryGet(Id);
		}

		void TreePanel::SetVisible(NxFr::GUID Id, bool State)
		{
			TreeItem* Item = GetItem(Id);
			if (!Item)
			{
				return;
			}

			Item = GetItem(Item->GetParent());

			if (State)
			{
				while (Item)
				{
					Item->Open(true);
					Item = GetItem(Item->GetParent());
				}
			}
			else
			{
				if (Item)
				{
					Item->Open(false);
				}
			}
		}

		bool TreePanel::IsVisible(NxFr::GUID Id)
		{
			TreeItem* Item = GetItem(Id);
			if (!Item)
			{
				return false;
			}

			bool Visible = true;
			Item = GetItem(Item->GetParent());

			while (Item)
			{
				Visible &= Item->IsOpen();
				Item = GetItem(Item->GetParent());
			}

			return Visible;
		}

		void TreePanel::AppendAction(const TreeAction& Action)
		{
			auto It = GetAction(Action.Name);
			if (It != Actions.End())
			{
				NX_LOG(Warning, Default, "Panel already contains actions %s", Action.Name.C());
				return;
			}

			Actions.AppendConstruct(Action);
			NxFr::ContainerUtility::Sort<TreeAction, NxFr::List<TreeAction>, NxFr::Sorting::MergeSortIndexed<TreeAction>>(Actions);
		}

		void TreePanel::RemoveAction(NxFr::StringView Name)
		{
			auto It = GetAction(Name);
			if (It == Actions.End())
			{
				NX_LOG(Warning, Default, "Panel doesn't contains actions %s", Name.C());
				return;
			}

			Actions.Remove(It.Id());
		}

		void TreePanel::OnInitialize()
		{
			Panel::OnInitialize();
			Menu.Initialize();

			Menu.AddMenuItem("Refresh", { this, &TreePanel::Refresh });
		}

		void TreePanel::OnShutdown()
		{
			Menu.Shutdown();
			Panel::OnShutdown();
		}

		void TreePanel::OnEnable()
		{
			Panel::OnEnable();
			Menu.Show();

			SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);

			Inputs = Application::GetSystem<InputSystem>();
		}

		void TreePanel::OnDisable()
		{
			Clear();

			Menu.Hide();
			Panel::OnDisable();
		}

		void TreePanel::OnDraw()
		{
			DrawHeader();
			DrawItem(Root);

			ProcessAction();
		}

		void TreePanel::OnCreateItem(NxFr::GUID Id)
		{
			Select(Id, true);
		}

		void TreePanel::OnDestroyItem(NxFr::GUID Id)
		{
			Select(Id, false, false, false, false);
			Filtered.TryRemove(Id);
			if (Root == Id)
			{
				Root = 0;
			}
		}

		void TreePanel::OnSelectItem(NxFr::GUID Id, bool State)
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
					Find(Filter);
				}
			}
			ImGui::EndMenuBar();

			TreeItem* Instance = GetItem(Selected);
			NxEn::GUI::Drawer<NxFr::String>::Property(Instance ? Instance->GetDescription() : "", "Selected", Visual);

			ImGui::Separator();
		}

		void TreePanel::DrawItem(NxFr::GUID Id)
		{
			TreeItem* Item = GetItem(Id);
			if (!Item)
			{
				return;
			}

			bool Browse = Filter.IsEmpty();
			bool ExpandChanged = false;
			bool Selected = IsSelected(Id);

			if (Browse || Filtered.TryGet(Id))
			{
				// Draw
				uint64 Flag = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow
					| (!Item->GetChild() ? ImGuiTreeNodeFlags_Leaf : 0)
					| (Selected ? ImGuiTreeNodeFlags_Selected : 0);

				if (Browse)
				{
					ImGui::SetNextItemOpen(Item->IsOpen(), ImGuiCond_Always);
					bool State = ImGui::TreeNodeEx(Item->GetLabel().C(), Flag);
					ExpandChanged = Item->IsOpen() != State;
					Item->Open(State);
				}
				else
				{
					ImGui::Selectable(Item->GetLabel().C(), Selected);
				}

				// Inputs
				if (ImGui::IsItemHovered())
				{
					if (Inputs->CheckButton(NxEn::Input::Button::MouseLeft, NxEn::Input::State::Pressed) && !ExpandChanged)
					{
						HandleSelection(Id, Selected);
					}
					if (Inputs->CheckButton(NxEn::Input::Button::MouseRight, NxEn::Input::State::Pressed))
					{
						ImGui::OpenPopup(Item->GetLabel().C());
					}
				}

				DrawAction(Item->GetLabel());
			}

			// Iterate
			if (Item->IsOpen())
			{
				if (Browse)
				{
					ImGui::TreePush(Item->GetLabel().C());
				}

				DrawItem(Item->GetChild());

				if (Browse)
				{
					ImGui::TreePop();
				}
			}

			DrawItem(Item->GetNext());
		}

		void TreePanel::DrawAction(NxFr::StringView Label)
		{
			if (ImGui::BeginPopup(Label.C()))
			{
				for (const auto& A : Actions)
				{
					if (ImGui::MenuItem(A.Name.C()))
					{
						Action = A.Name;
						break;
					}
				}

				ImGui::EndPopup();
			}
		}

		void TreePanel::HandleSelection(NxFr::GUID Id, bool Selected)
		{
			if (Inputs->CheckModifier(NxEn::Input::Modifier::Ctrl))
			{
				Select(Id, !Selected, true, false);
			}
			else if (Inputs->CheckModifier(NxEn::Input::Modifier::Shift))
			{
				Select(Id, !Selected, true, true);
			}
			else
			{
				Select(Id, true, false, false);
			}
		}

		void TreePanel::ProcessAction()
		{
			if (Action.IsEmpty())
			{
				return;
			}

			auto It = GetAction(Action);
			if (It != Actions.End())
			{
				It->Action.Invoke();
			}

			Action.Clear();
		}

		NxFr::GUID TreePanel::GetIterator(NxFr::GUID Id)
		{
			TreeItem* Item = GetItem(Id);
			if (!Item)
			{
				return 0;
			}

			if (Item->GetChild())
			{
				return Item->GetChild();
			}

			if (Item->GetNext())
			{
				return Item->GetNext();
			}

			TreeItem* Parent = GetItem(Item->GetParent());
			while (Parent && !Parent->GetNext())
			{
				Parent = GetItem(Parent->GetParent());
			}

			if (Parent)
			{
				return Parent->GetNext();
			}

			return 0;
		}

		NxFr::List<TreeAction>::I TreePanel::GetAction(NxFr::StringView Name)
		{
			return NxFr::ContainerUtility::Where<TreeAction>(Actions, [&](const TreeAction& Element) { return Element.Name == Name; });
		}
	}
}
