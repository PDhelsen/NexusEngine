#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyItem : public NxEn::TreeItem
	{
		friend class HierarchyManager;

	public:
		NX_OBJECT(HierarchyItem)

		NxFr::GUID GetId() const override { return Target->GetId(); }
		NxFr::StringView GetName() const override { return Target->GetName(); }
		NxFr::StringView GetLabel() const override { CacheLabel(); return Label; }
		NxFr::StringView GetDescription() const override { return Target->GetName(); }
		NxFr::StringId GetType() const override { return NxEn::GameObject::GetClassType(); }

		NxFr::Handle<NxEn::GameObject> GetTarget() const { return Target; }

		NxFr::GUID GetParent() const override { return Target->GetParent() ? Target->GetParent()->GetId() : 0; }
		NxFr::GUID GetPrevious() const override { return Target->GetPrevious() ? Target->GetPrevious()->GetId() : 0; }
		NxFr::GUID GetNext() const override { return Target->GetNext() ? Target->GetNext()->GetId() : 0; }
		NxFr::GUID GetChild() const override { return Target->GetChild() ? Target->GetChild()->GetId() : 0; }

		bool IsOpen() const override { return Opened; }
		void Open(bool State) override { Opened = State; }

		bool Compare(const TreeItem& Other) const override;

	private:
		HierarchyItem();
		~HierarchyItem();

		void CacheLabel() const;

		NxFr::Handle<NxEn::GameObject> Target;
		bool Opened;

		mutable NxFr::String Label;
		mutable NxFr::Tuple<NxFr::String, NxFr::GUID> Cache;
	};
}
