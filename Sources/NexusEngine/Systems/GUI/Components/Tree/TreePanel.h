#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEn
{
	class TreePanel : public GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, TreePanel)

		NX_ENGINE_API TreePanel();
		NX_ENGINE_API virtual ~TreePanel();

		NX_ENGINE_API virtual void Clear();
		NX_ENGINE_API virtual void Refresh();

		NX_ENGINE_API virtual void Find(NxFr::StringView Query);
		NX_ENGINE_API virtual void Select(TreeItem* Item);

		template<typename T>
		void AppendAction();
		template<typename T>
		void RemoveAction();
		template<typename T>
		void RunAction();

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnEnable() override;
		NX_ENGINE_API void OnDisable() override;
		NX_ENGINE_API void OnGui(float TimeStep) override;

	protected:
		NX_ENGINE_API virtual TreeItem* FetchRootItem() = 0;
		NX_ENGINE_API virtual void OnCreateItem(TreeItem* Item);
		NX_ENGINE_API virtual void OnDestroyItem(TreeItem* Item);
		NX_ENGINE_API virtual void OnSelectItem(TreeItem* Item, bool State);

		NX_ENGINE_API virtual void DrawHeader(float TimeStep);
		NX_ENGINE_API virtual void DrawItem(float TimeStep, TreeItem* Item);
		NX_ENGINE_API virtual void DrawContext(TreeItem* Item);
		NX_ENGINE_API virtual void OpenContext(TreeItem* Item);
		NX_ENGINE_API virtual void HandleSelection(TreeItem* Item);

		NX_ENGINE_API virtual void FindItem();
		NX_ENGINE_API virtual void SelectItem(TreeItem* Item, bool State, bool Additive, bool List);
		NX_ENGINE_API virtual void ShowItem(TreeItem* Item);
		NX_ENGINE_API virtual bool IsItemVisible(TreeItem* Item);

		NX_ENGINE_API virtual void ProcessAction();
		NX_ENGINE_API virtual NxFr::Array<TreeItem*> GatherActionItems(TreeAction* Action);
		NX_ENGINE_API virtual void GatherActionItems(TreeItem* Item, NxFr::Set<TreeItem*>& Result, bool Recursive);
		NX_ENGINE_API virtual void SortActions();

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

