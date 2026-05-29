#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyPanel : public NxEn::TreePanel
	{
		friend class HierarchyManager;
		friend class HierarchyEditContext;

	public:
		NX_OBJECT(HierarchyPanel)

		void Refresh() override;

		void Show(NxFr::Handle<NxEn::GameObject> Target);
		void SelectGameObject(NxFr::Handle<NxEn::GameObject> Target);

	protected:
		HierarchyPanel(HierarchyManager* Manager);
		virtual ~HierarchyPanel();

		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		NxEn::TreeItem* FetchRootItem() override;
		void OnDestroyItem(NxEn::TreeItem* Item) override;
		void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

		void FindItem() override;

	private:
		class EditSystem* Edit;

		class HierarchyManager* Manager;
		class HierarchyEditContext* Context;
	};
}
