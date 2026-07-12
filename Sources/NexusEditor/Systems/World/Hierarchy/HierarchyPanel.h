#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

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

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		NxEn::TreeItem* FetchRootItem() override;
		void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

		void FindItem() override;

	private:
		HierarchyManager* Manager;
	};
}
