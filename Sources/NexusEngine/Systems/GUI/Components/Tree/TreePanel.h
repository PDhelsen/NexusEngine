#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEn
{
	class NX_ENGINE_API TreePanel : public GUI::Panel
	{
	public:
		NX_OBJECT(TreePanel)

		TreePanel();
		virtual ~TreePanel();

		virtual void Clear();
		virtual void Refresh();

		virtual void Find(NxFr::StringView Query);
		virtual void Select(TreeItem* Item);

		template<typename T>
		void AppendAction();
		template<typename T>
		void RemoveAction();
		template<typename T>
		void RunAction();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	protected:
		virtual TreeItem* FetchRootItem() = 0;
		virtual void OnCreateItem(TreeItem* Item);
		virtual void OnDestroyItem(TreeItem* Item);
		virtual void OnSelectItem(TreeItem* Item, bool State);

		virtual void DrawHeader();
		virtual void DrawItem(TreeItem* Item);
		virtual void DrawContext(TreeItem* Item);
		virtual void OpenContext(TreeItem* Item);
		virtual void HandleSelection(TreeItem* Item);

		virtual void FindItem();
		virtual void SelectItem(TreeItem* Item, bool State, bool Additive, bool List);
		virtual void ShowItem(TreeItem* Item);
		virtual bool IsItemVisible(TreeItem* Item);

		virtual void ProcessAction();
		virtual NxFr::Array<TreeItem*> GatherActionItems(TreeAction* Action);
		virtual void GatherActionItems(TreeItem* Item, NxFr::Set<TreeItem*>& Result, bool Recursive);
		virtual void SortActions();

	protected:
		GUI::Menu Menu;
		GUI::Style Style;

		NxEn::InputSystem* Inputs;

		TreeItem* Root;
		NxFr::Set<TreeItem*> Selection;
		TreeItem* Selected;
		NxFr::Set<TreeItem*> Filtered;
		NxFr::String Filter;
		NxFr::List<TreeAction*> Actions;
		TreeAction* ActionRequested;
	};

	template<typename T>
	inline void TreePanel::AppendAction()
	{
		Actions.Append(new T());
		SortActions();
	}

	template<typename T>
	inline void TreePanel::RemoveAction()
	{
		uint64 Index = 0;
		for (; Index < Actions.GetCount(); ++Index)
		{
			if (Actions[Index]->GetObjectType() == T::GetClassType())
			{
				break;
			}
		}

		if (Index < Actions.GetCount())
		{
			delete Actions[Index];
			Actions.Remove(Index);
			SortActions();
		}
	}

	template<typename T>
	inline void TreePanel::RunAction()
	{
		for (uint64 Index = 0; Index < Actions.GetCount(); ++Index)
		{
			TreeAction* Action = Actions[Index];
			if (Actions[Index]->GetObjectType() == T::GetClassType())
			{
				ActionRequested = Action;
				return;
			}
		}
	}
}

