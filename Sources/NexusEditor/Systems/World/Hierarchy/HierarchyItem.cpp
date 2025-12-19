#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyItem)

	HierarchyItem::HierarchyItem(NxFr::Handle<NxEn::GameObject> GameObject)
		: GameObject(GameObject), ImGuiText(), Selected(false), Expanded(false)
	{
	}

	HierarchyItem::~HierarchyItem()
	{
	}

	void HierarchyItem::EnsureImGuiText()
	{
		if (NxFr::StringUtility::Split(ImGuiText, "##") == GetGameObject()->GetName())
		{
			return;
		}

		ImGuiText = GameObject->GetName() + "##" + NxFr::StringUtility::ToString(GameObject->GetId());
	}
}
