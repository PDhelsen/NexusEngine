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
		friend class HierarchyItem;

	public:
		HierarchyManager();
		~HierarchyManager();

		void Refresh();
		void Select(NxFr::GUID Id);
		void Select(NxFr::Handle<NxEn::GameObject> Instance);

	private:
		void OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId);

		HierarchyItem* FetchItems();
		void ClearItems();
		void AppendItem(NxFr::Handle<NxEn::GameObject> Instance);
		void RemoveItem(NxFr::Handle<NxEn::GameObject> Instance);
		HierarchyItem* GetItem(NxFr::Handle<NxEn::GameObject> Instance);
		HierarchyItem* GetItem(NxFr::GUID Id);
		void SelectItem(NxFr::Handle<NxEn::GameObject> Instance, bool State, NxFr::StringId SelectionId);

		NxEn::WorldSystem* Worlds;
		EditSystem* Edit;

		NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Items;
		HierarchyItem* Root;
		HierarchyPanel* Panel;
		HierarchyEditContext Context;

		NxFr::StringId SelectionContextId;
	};
}

