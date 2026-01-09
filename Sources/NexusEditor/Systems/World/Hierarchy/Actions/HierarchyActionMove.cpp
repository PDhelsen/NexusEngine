#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionMove.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionMove)

	HierarchyActionMove::HierarchyActionMove()
		: TreeAction("Move", 4, false, true)
	{
	}

	HierarchyActionMove::~HierarchyActionMove()
	{
	}

	void HierarchyActionMove::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxFr::Handle<NxEn::GameObject> Parent = static_cast<HierarchyItem*>(Items[0])->GetGameObject();
		NxEn::World* World = Parent->GetWorld();

		for (uint64 Index = 1; Index < Items.GetCount(); Index++)
		{
			NxFr::Handle<NxEn::GameObject> Item = static_cast<HierarchyItem*>(Items[Index])->GetGameObject();
			World->AttachGameObject(Item, Parent);
		}
	}
}
