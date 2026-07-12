#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	static const uint64 PrefixSize = 2;
	static const uint64 ImGuiSplitSize = 2;

	HierarchyItem::HierarchyItem(HierarchyManager* Manager, NxFr::Handle<NxEn::GameObject> Target)
		: Manager(Manager), Target(Target)
	{
	}

	HierarchyItem::~HierarchyItem()
	{
	}

	void HierarchyItem::OnDraw()
	{
		NxFr::StringView Name = Target->GetName();

		bool RecacheImGui =
			NxFr::StringCApi::Compare(ImGuiText.C() + PrefixSize, Name.C(), Name.GetCount()) != 0 ||
			NxFr::StringCApi::Compare(ImGuiText.C() + PrefixSize + Name.GetCount(), "##", ImGuiSplitSize) != 0;

		if (RecacheImGui)
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
			"G";

		ImGuiText = Prefix + " " + Target->GetName() + "##" + NxFr::StringUtility::ToString(Target->GetId());
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
