#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class HierarchyItem : public NxEn::TreeItem
	{
		friend class HierarchyManager;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyItem)

		NEXUS_EDITOR_API NxFr::StringView GetName() const override { return ImGuiText; }
		NEXUS_EDITOR_API NxFr::StringView GetDescription() const { return GetItemName(); }

		NEXUS_EDITOR_API NxFr::StringView GetItemName() const override { return Target->GetName(); }
		NEXUS_EDITOR_API NxFr::StringId GetItemType() const override { return Target->GetObjectType(); }
		NEXUS_EDITOR_API NxFr::GUID GetItemId() const override { return Target->GetId(); }

		NEXUS_EDITOR_API NxFr::Handle<NxEn::GameObject> GetTarget() const { return Target; }

	private:
		NEXUS_EDITOR_API HierarchyItem(HierarchyManager* Manager, NxFr::Handle<NxEn::GameObject> Target);
		NEXUS_EDITOR_API virtual ~HierarchyItem();

		NEXUS_EDITOR_API void OnTick(float TimeStep) override;

		NEXUS_EDITOR_API int8 Compare(const TreeItem& Other) const override;
		NEXUS_EDITOR_API void CacheImGuiText() override;

		NEXUS_EDITOR_API HierarchyItem* GetParent() const override;
		NEXUS_EDITOR_API HierarchyItem* GetPrevious() const override;
		NEXUS_EDITOR_API HierarchyItem* GetNext() const override;
		NEXUS_EDITOR_API HierarchyItem* GetChild() const override;

	private:
		HierarchyManager* Manager;
		NxFr::Handle<NxEn::GameObject> Target;

		NxFr::String Name;
		NxFr::GUID ReferenceId;
	};
}
