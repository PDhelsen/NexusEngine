#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/Tree/TreeItem.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyItem : public NxEn::TreeItem
	{
		friend class HierarchyManager;

	public:
		NX_OBJECT(HierarchyItem)

		NxFr::StringView GetName() const override { return ImGuiText; }
		NxFr::StringView GetDescription() const { return GetItemName(); }

		NxFr::StringView GetItemName() const override { return Target->GetName(); }
		NxFr::StringId GetItemType() const override { return Target->GetObjectType(); }
		NxFr::GUID GetItemId() const override { return Target->GetId(); }

		NxFr::Handle<NxEn::GameObject> GetTarget() const { return Target; }

	private:
		HierarchyItem(HierarchyManager* Manager, NxFr::Handle<NxEn::GameObject> Target);
		virtual ~HierarchyItem();

		void OnDraw() override;

		int8 Compare(const TreeItem& Other) const override;
		void CacheImGuiText() override;

		HierarchyItem* GetParent() const override;
		HierarchyItem* GetPrevious() const override;
		HierarchyItem* GetNext() const override;
		HierarchyItem* GetChild() const override;

	private:
		HierarchyManager* Manager;
		NxFr::Handle<NxEn::GameObject> Target;
		NxFr::Tuple<NxFr::String, NxFr::GUID> Cache;
	};
}
