#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyManager
	{
		friend class HierarchyPanel;
		friend class HierarchyEditContext;

	public:
		HierarchyManager();
		~HierarchyManager();

		HierarchyPanel* CreatePanel();
		void DestroyPanel(HierarchyPanel* Instance);

		HierarchyItem* Convert(NxFr::Handle<NxEn::GameObject> Instance);

	private:
		void OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId);

		void FetchItems();
		void ClearItems();
		void AppendItem(NxFr::Handle<NxEn::GameObject> Target);
		void RemoveItem(NxFr::Handle<NxEn::GameObject> Target);

	private:
		NxEn::WorldSystem* Worlds;

		NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Items;
		NxFr::Dictionary<NxFr::GUID, HierarchyPanel*> Panels;
	};
}

