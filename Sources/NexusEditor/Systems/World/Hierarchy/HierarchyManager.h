#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyManager
	{
		friend class HierarchyPanel;
		friend class HierarchyEditContext;

	public:
		NX_NOCOPY_NOMOVE(HierarchyManager)
		HierarchyManager();
		~HierarchyManager();

		HierarchyItem* GetItem(NxFr::GUID GameObjectId);

	private:
		void FetchItems();
		void ClearItems();

		void AppendItem(NxFr::Handle<NxEn::GameObject> Instance);
		void RemoveItem(NxFr::Handle<NxEn::GameObject> Instance);
		void SelectItem(HierarchyItem* Item, bool State);

		void OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId);
		void SetEditContext();

		NxEn::WorldSystem* Worlds;
		EditSystem* Edit;

		NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Items;
		HierarchyPanel* Panel;
		HierarchyEditContext* Context;
	};
}

