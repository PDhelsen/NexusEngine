#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class HierarchyItem : public NxEn::TreeItem
	{
		friend class HierarchyManager;

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyItem)

		NX_EDITOR_API NxFr::StringView GetName() const override { return ImGuiText; }
		NX_EDITOR_API NxFr::StringView GetDescription() const { return GetItemName(); }

		NX_EDITOR_API NxFr::StringView GetItemName() const override { return Target->GetName(); }
		NX_EDITOR_API NxFr::StringId GetItemType() const override { return Target->GetObjectType(); }
		NX_EDITOR_API NxFr::GUID GetItemId() const override { return Target->GetId(); }

		NX_EDITOR_API NxFr::Handle<NxEn::GameObject> GetTarget() const { return Target; }

	private:
		NX_EDITOR_API HierarchyItem(HierarchyManager* Manager, NxFr::Handle<NxEn::GameObject> Target);
		NX_EDITOR_API virtual ~HierarchyItem();

		NX_EDITOR_API void OnTick(float TimeStep) override;

		NX_EDITOR_API int8 Compare(const TreeItem& Other) const override;
		NX_EDITOR_API void CacheImGuiText() override;

		NX_EDITOR_API HierarchyItem* GetParent() const override;
		NX_EDITOR_API HierarchyItem* GetPrevious() const override;
		NX_EDITOR_API HierarchyItem* GetNext() const override;
		NX_EDITOR_API HierarchyItem* GetChild() const override;

	private:
		HierarchyManager* Manager;
		NxFr::Handle<NxEn::GameObject> Target;

		NxFr::String Name;
		NxFr::GUID ReferenceId;
	};
}
