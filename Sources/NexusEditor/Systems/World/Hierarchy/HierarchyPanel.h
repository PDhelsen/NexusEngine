#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class HierarchyPanel : public NxEn::TreePanel
	{
		friend class HierarchyManager;
		friend class HierarchyEditContext;

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyPanel)

		NX_EDITOR_API void Refresh() override;

		NX_EDITOR_API void Show(NxFr::Handle<NxEn::GameObject> Target);
		NX_EDITOR_API void SelectGameObject(NxFr::Handle<NxEn::GameObject> Target);

	protected:
		NX_EDITOR_API HierarchyPanel(HierarchyManager* Manager);
		NX_EDITOR_API virtual ~HierarchyPanel();

		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnShutdown() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnDisable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

		NX_EDITOR_API NxEn::TreeItem* FetchRootItem() override;
		NX_EDITOR_API void OnDestroyItem(NxEn::TreeItem* Item) override;
		NX_EDITOR_API void OnSelectItem(NxEn::TreeItem* Item, bool State) override;

		NX_EDITOR_API void FindItem() override;

	private:
		class EditSystem* Edit;

		class HierarchyManager* Manager;
		class HierarchyEditContext* Context;
	};
}
