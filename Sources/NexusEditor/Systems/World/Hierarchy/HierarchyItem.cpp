#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyItem::HierarchyItem(HierarchyManager* Manager, NxFr::Handle<NxEn::GameObject> Target)
		: Manager(Manager), Target(Target), Cache("", 0)
	{
	}

	HierarchyItem::~HierarchyItem()
	{
	}

	void HierarchyItem::OnDraw()
	{
		if (Cache.GetFirst() != Target->GetName() || Cache.GetSecond() != Target->GetTemplateId())
		{
			CacheImGuiText();
		}
	}

	int8 HierarchyItem::Compare(const TreeItem& Other) const
	{
		return Target->GetOrderIndex() - static_cast<const HierarchyItem&>(Other).Target->GetOrderIndex();
	}

	void HierarchyItem::CacheImGuiText()
	{
		NxFr::StringView Prefix =
			Target == Target->GetWorld()->GetRoot() ? "W" :
			Target->GetAssetId() ? "P" :
			"G";

		ImGuiText = Prefix + " " + Target->GetName() + "##" + NxFr::StringUtility::ToString(Target->GetId());

		Cache.SetFirst(Target->GetName());
		Cache.SetSecond(Target->GetTemplateId());
	}

	HierarchyItem* HierarchyItem::GetParent() const
	{
		return Manager->GetItem(Target->GetParent());
	}

	HierarchyItem* HierarchyItem::GetPrevious() const
	{
		return Manager->GetItem(Target->GetPrevious());
	}

	HierarchyItem* HierarchyItem::GetNext() const
	{
		return Manager->GetItem(Target->GetNext());
	}

	HierarchyItem* HierarchyItem::GetChild() const
	{
		return Manager->GetItem(Target->GetChild());
	}
}
