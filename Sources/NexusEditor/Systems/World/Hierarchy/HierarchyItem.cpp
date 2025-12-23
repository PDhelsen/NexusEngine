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

		NxFr::StringView Prefix =
			GameObject == GameObject->GetWorld()->GetRootGameObject() ? "W" : "G";

		ImGuiText = Prefix + " " + GameObject->GetName() + "##" + NxFr::StringUtility::ToString(GameObject->GetId());
	}
}
