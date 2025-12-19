#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionMove.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionMove)

	HierarchyActionMove::HierarchyActionMove()
		: HierarchyAction("Move", 4, false, true)
	{
	}

	HierarchyActionMove::~HierarchyActionMove()
	{
	}

	void HierarchyActionMove::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		Move(Items[0]->GetWorld(), Items, Items[0]);
	}

	void HierarchyActionMove::Move(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items, NxFr::Handle<NxEn::GameObject> Parent) const
	{
		for (uint64 Index = 1; Index < Items.GetCount(); Index++)
		{
			Move(World, Items[Index], Parent);
		}
	}

	void HierarchyActionMove::Move(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item, NxFr::Handle<NxEn::GameObject> Parent) const
	{
		World->AttachGameObject(Item, Parent);
	}
}
