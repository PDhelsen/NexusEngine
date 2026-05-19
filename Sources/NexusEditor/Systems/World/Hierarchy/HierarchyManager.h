#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class HierarchyManager
	{
		friend class HierarchyPanel;
		friend class HierarchyEditContext;

	public:
		NX_EDITOR_API HierarchyManager();
		NX_EDITOR_API ~HierarchyManager();

		NX_EDITOR_API HierarchyPanel* CreatePanel();
		NX_EDITOR_API void DestroyPanel(HierarchyPanel* Instance);

		NX_EDITOR_API HierarchyItem* Convert(NxFr::Handle<NxEn::GameObject> Instance);

	private:
		NX_EDITOR_API void OnHierarchyChanged(NxFr::StringId EventId, NxFr::GUID WorldId, NxFr::GUID GameObjectId);

		NX_EDITOR_API void FetchItems();
		NX_EDITOR_API void ClearItems();
		NX_EDITOR_API void AppendItem(NxFr::Handle<NxEn::GameObject> Target);
		NX_EDITOR_API void RemoveItem(NxFr::Handle<NxEn::GameObject> Target);

	private:
		NxEn::WorldSystem* Worlds;

		NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Items;
		NxFr::Dictionary<NxFr::GUID, HierarchyPanel*> Panels;
	};
}

