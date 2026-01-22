#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class HierarchyItem : public NxEn::TreeItem
	{
		friend class HierarchyPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyItem)

		NEXUS_EDITOR_API NxFr::StringView GetName() const override { return ImGuiText; }
		NEXUS_EDITOR_API NxFr::StringView GetDescription() const { return GetItemName(); }

		NEXUS_EDITOR_API NxFr::StringView GetItemName() const override { return Target->GetName(); }
		NEXUS_EDITOR_API NxFr::StringId GetItemType() const override { return Target->GetObjectType(); }
		NEXUS_EDITOR_API NxFr::GUID GetItemId() const override { return Target->GetId(); }

		NEXUS_EDITOR_API NxFr::Handle<NxEn::GameObject> GetTarget() const { return Target; }

	private:
		NEXUS_EDITOR_API HierarchyItem(NxFr::Handle<NxEn::GameObject> Target);
		NEXUS_EDITOR_API virtual ~HierarchyItem();

		NEXUS_EDITOR_API void OnTick(float TimeStep) override;

		NEXUS_EDITOR_API int8 Compare(const TreeItem& Other) const override;
		NEXUS_EDITOR_API void CacheImGuiText() override;

		NEXUS_EDITOR_API HierarchyItem* GetParent() const override { return Convert(Target->GetParent()); }
		NEXUS_EDITOR_API HierarchyItem* GetPrevious() const override { return Convert(Target->GetPrevious()); }
		NEXUS_EDITOR_API HierarchyItem* GetNext() const override { return Convert(Target->GetNext()); }
		NEXUS_EDITOR_API HierarchyItem* GetChild() const override { return Convert(Target->GetChild()); }

		NEXUS_EDITOR_API static HierarchyItem* Convert(NxFr::Handle<NxEn::GameObject> Target);

	private:
		NxFr::Handle<NxEn::GameObject> Target;

		NxFr::String Name;
		NxFr::GUID ReferenceId;
	};
}
