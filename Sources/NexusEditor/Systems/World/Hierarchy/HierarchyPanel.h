#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"

namespace NxEd
{
	class HierarchyPanel : public NxEn::TreePanel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyPanel)

		NEXUS_EDITOR_API void Clear() override;
		NEXUS_EDITOR_API void Refresh() override;

		NEXUS_EDITOR_API void SetRoot(NxFr::Handle<NxEn::GameObject> Target);
		NEXUS_EDITOR_API void SelectGameObject(NxFr::Handle<NxEn::GameObject> Target);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;

		NEXUS_EDITOR_API NxEn::TreeItem* FetchRootItem() override;
		NEXUS_EDITOR_API void OnHierarchyChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId);
		NEXUS_EDITOR_API void AppendItem(NxFr::Handle<NxEn::GameObject> Target);
		NEXUS_EDITOR_API void RemoveItem(NxFr::Handle<NxEn::GameObject> Target);

		NEXUS_EDITOR_API void Find() override;

	private:
		NxEn::WorldSystem* Worlds;
	};
}
