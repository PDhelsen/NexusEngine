#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/Tree/TreePanel.h"

namespace NxEd
{
	class HierarchyManager;
	class HierarchyItem;

	class NX_EDITOR_API HierarchyPanel : public NxEn::TreePanel
	{
		friend class HierarchyManager;

	public:
		NX_OBJECT(HierarchyPanel)

		void Refresh() override;
		void Select(NxFr::GUID Id);
		void Select(NxFr::Handle<NxEn::GameObject> Target);

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		NxEn::TreeItem* FetchRootItem() override;
		void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

		void FindItem() override;

	private:
		HierarchyPanel(HierarchyManager* Manager, HierarchyItem* Root);
		~HierarchyPanel();

		HierarchyManager* Manager;
	};
}
