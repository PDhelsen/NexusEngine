#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class HierarchyPanel : public NxEn::TreePanel
	{
		friend class HierarchyManager;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyPanel)

		NEXUS_EDITOR_API void Refresh() override;

		NEXUS_EDITOR_API void SetRoot(NxFr::Handle<NxEn::GameObject> Target);
		NEXUS_EDITOR_API void SelectGameObject(NxFr::Handle<NxEn::GameObject> Target);

	protected:
		NEXUS_EDITOR_API HierarchyPanel(HierarchyManager* Manager);
		NEXUS_EDITOR_API virtual ~HierarchyPanel();

		NEXUS_EDITOR_API void OnInitialize() override;

		NEXUS_EDITOR_API NxEn::TreeItem* FetchRootItem() override;

		NEXUS_EDITOR_API void FindItem() override;

	private:
		HierarchyManager* Manager;
	};
}
