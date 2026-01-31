#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEn
{
	class TreePanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, TreePanel)

		NEXUS_ENGINE_API TreePanel();
		NEXUS_ENGINE_API virtual ~TreePanel();

		NEXUS_ENGINE_API virtual void Clear();
		NEXUS_ENGINE_API virtual void Refresh();

		NEXUS_ENGINE_API virtual void Find(NxFr::StringView Query);
		NEXUS_ENGINE_API virtual void Select(TreeItem* Item);

		template<typename T>
		void AppendAction();
		template<typename T>
		void RemoveAction();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnDisable() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

	protected:
		NEXUS_ENGINE_API virtual TreeItem* FetchRootItem() = 0;
		NEXUS_ENGINE_API virtual void OnCreateItem(TreeItem* Item);
		NEXUS_ENGINE_API virtual void OnDestroyItem(TreeItem* Item);

		NEXUS_ENGINE_API virtual void DrawHeader(float TimeStep);
		NEXUS_ENGINE_API virtual void DrawItem(float TimeStep, TreeItem* Item);
		NEXUS_ENGINE_API virtual void DrawContext(TreeItem* Item);
		NEXUS_ENGINE_API virtual void OpenContext(TreeItem* Item);
		NEXUS_ENGINE_API virtual void HandleSelection(TreeItem* Item);

		NEXUS_ENGINE_API virtual void FindItem();
		NEXUS_ENGINE_API virtual void SelectItem(TreeItem* Item, bool Additive, bool List);
		NEXUS_ENGINE_API virtual void ShowItem(TreeItem* Item);
		NEXUS_ENGINE_API virtual bool IsItemVisible(TreeItem* Item);

		NEXUS_ENGINE_API virtual void ProcessAction();
		NEXUS_ENGINE_API virtual NxFr::Array<TreeItem*> GatherActionItems(TreeAction* Action);
		NEXUS_ENGINE_API virtual void GatherActionItems(TreeItem* Item, NxFr::Set<TreeItem*>& Result, bool Recursive);
		NEXUS_ENGINE_API virtual void SortActions();

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
}

