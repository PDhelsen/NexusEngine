#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDuplicate.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionDuplicate)

	HierarchyActionDuplicate::HierarchyActionDuplicate()
		: HierarchyAction("Duplicate", 2)
	{
	}

	HierarchyActionDuplicate::~HierarchyActionDuplicate()
	{
	}

	void HierarchyActionDuplicate::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		Duplicate(Items[0]->GetWorld(), Items);
	}

	void HierarchyActionDuplicate::Duplicate(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const
	{
		for (auto& Item : Items)
		{
			Duplicate(World, Item);
		}
	}

	void HierarchyActionDuplicate::Duplicate(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item) const
	{
		World->DuplicateGameObject(Item);
	}
}
