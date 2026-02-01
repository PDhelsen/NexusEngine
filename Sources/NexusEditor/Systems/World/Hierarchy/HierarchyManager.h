#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class HierarchyManager
	{
	public:
		NEXUS_EDITOR_API HierarchyManager();
		NEXUS_EDITOR_API ~HierarchyManager();

		NEXUS_EDITOR_API HierarchyItem* Convert(NxFr::Handle<NxEn::GameObject> Instance);

	private:
		NEXUS_EDITOR_API void OnHierarchyChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId);

		NEXUS_EDITOR_API void FetchItems();
		NEXUS_EDITOR_API void ClearItems();
		NEXUS_EDITOR_API void AppendItem(NxFr::Handle<NxEn::GameObject> Target);
		NEXUS_EDITOR_API void RemoveItem(NxFr::Handle<NxEn::GameObject> Target);

	private:
		NxEn::WorldSystem* Worlds;

		NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Items;
		NxFr::Dictionary<NxFr::GUID, HierarchyPanel*> Panels;
	};
}

