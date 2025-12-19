#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDelete.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionDelete)

	HierarchyActionDelete::HierarchyActionDelete()
		: HierarchyAction("Delete", 5)
	{
	}

	HierarchyActionDelete::~HierarchyActionDelete()
	{
	}

	void HierarchyActionDelete::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		Delete(Items[0]->GetWorld(), Items);
	}

	void HierarchyActionDelete::Delete(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const
	{
		for (auto& Item : Items)
		{
			Delete(World, Item);
		}
	}

	void HierarchyActionDelete::Delete(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item) const
	{
		World->DestroyGameObject(Item);
	}
}
