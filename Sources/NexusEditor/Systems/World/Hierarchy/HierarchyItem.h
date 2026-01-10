#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeItem.h"

namespace NxEd
{
	class HierarchyItem : public NxEn::TreeItem
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyItem)

		NEXUS_EDITOR_API HierarchyItem(NxFr::Handle<NxEn::GameObject> GameObject, const NxFr::Delegate<TreeItem*(NxFr::Handle<NxEn::GameObject>)>& Convert);
		NEXUS_EDITOR_API virtual ~HierarchyItem();

		NEXUS_EDITOR_API virtual TreeItem* GetParent() const { return Convert.Invoke(GameObject->GetParent()); }
		NEXUS_EDITOR_API virtual void SetParent(TreeItem* Instance) { NEXUS_ASSERT(false, System, "Not supported"); }
		NEXUS_EDITOR_API virtual TreeItem* GetPrevious() const { return Convert.Invoke(GameObject->GetPrevious()); }
		NEXUS_EDITOR_API virtual void SetPrevious(TreeItem* Instance) { NEXUS_ASSERT(false, System, "Not supported"); }
		NEXUS_EDITOR_API virtual TreeItem* GetNext() const { return Convert.Invoke(GameObject->GetNext()); }
		NEXUS_EDITOR_API virtual void SetNext(TreeItem* Instance) { NEXUS_ASSERT(false, System, "Not supported"); }
		NEXUS_EDITOR_API virtual TreeItem* GetChild() const { return Convert.Invoke(GameObject->GetChild()); }
		NEXUS_EDITOR_API virtual void SetChild(TreeItem* Instance) { NEXUS_ASSERT(false, System, "Not supported"); }

		NEXUS_EDITOR_API NxFr::StringView GetDescription() const override { return GetName(); }

		NEXUS_EDITOR_API NxFr::GUID GetId() const override { return GameObject->GetId(); }
		NEXUS_EDITOR_API NxFr::StringView GetName() const override { return GameObject->GetName(); }
		NEXUS_EDITOR_API NxFr::Handle<NxEn::GameObject> GetGameObject() const { return GameObject; }

	protected:
		NEXUS_EDITOR_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NEXUS_EDITOR_API void GenerateImGuiText() override;
		NEXUS_EDITOR_API int8 Compare(const TreeItem& Other) const override;

	private:
		NxFr::StringView Name;
		NxFr::GUID Reference;

		NxFr::Handle<NxEn::GameObject> GameObject;
		NxFr::Delegate<TreeItem*(NxFr::Handle<NxEn::GameObject>)> Convert;
	};
}
