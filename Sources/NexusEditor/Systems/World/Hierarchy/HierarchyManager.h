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

		HierarchyPanel* CreatePanel(NxEn::World* Target);
		void DestroyPanel(NxEn::World* Target);

	private:
		void OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId);

		void FetchItems();
		void ClearItems();
		void AppendItem(NxFr::Handle<NxEn::GameObject> Instance);
		void RemoveItem(NxFr::Handle<NxEn::GameObject> Instance);
		void SelectItem(NxFr::Handle<NxEn::GameObject> Instance, bool State, NxFr::StringId SelectionId);

		HierarchyItem* GetItem(NxFr::Handle<NxEn::GameObject> Instance);
		HierarchyItem* GetItem(NxFr::GUID Id);
		void SetEditContext(NxFr::GUID Id);

		NxEn::WorldSystem* Worlds;
		EditSystem* Edit;

		NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Items;
		NxFr::Dictionary<NxFr::GUID, HierarchyPanel*> Panels;
		NxFr::Dictionary<NxFr::GUID, HierarchyEditContext*> Contexts;

		NxFr::StringId SelectionContextId;
	};
}

