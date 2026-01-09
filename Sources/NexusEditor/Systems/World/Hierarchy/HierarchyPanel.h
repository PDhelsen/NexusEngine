#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreePanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	class HierarchyPanel : public NxEn::TreePanel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyPanel)

		NEXUS_EDITOR_API void RefreshWorld();

		NEXUS_EDITOR_API void SelectWorld(NxFr::StringId Id);
		NEXUS_EDITOR_API void SelectGameObject(NxFr::Handle<NxEn::GameObject> Target);

		NxFr::GUID GetWorldId() { return WorldsIds[WorldIndex]; }

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;

	private:
		NEXUS_EDITOR_API HierarchyItem* GetItem(NxFr::Handle<NxEn::GameObject> Instance);
		NEXUS_EDITOR_API HierarchyItem* FetchItems() override;
		NEXUS_EDITOR_API void AppendItem(NxFr::Handle<NxEn::GameObject> Instance);
		NEXUS_EDITOR_API void RemoveItem(NxFr::Handle<NxEn::GameObject> Instance);
		NEXUS_EDITOR_API void OnGameObjectChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId);

		NEXUS_EDITOR_API void Find() override;

		NxEn::World* GetWorld() { return Worlds->GetWorld(WorldsIds[WorldIndex]); }

	private:
		NxEn::WorldSystem* Worlds;

		NxFr::Array<NxFr::StringView> WorldsLabels;
		NxFr::Array<NxFr::GUID> WorldsIds;
		uint64 WorldIndex;
	};
}
