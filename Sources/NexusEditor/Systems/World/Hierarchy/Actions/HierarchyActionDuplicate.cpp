#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionDuplicate.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionDuplicate)

	HierarchyActionDuplicate::HierarchyActionDuplicate()
		: TreeAction("Duplicate", 2, false, false)
	{
	}

	HierarchyActionDuplicate::~HierarchyActionDuplicate()
	{
	}

	void HierarchyActionDuplicate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::World* World = static_cast<HierarchyItem*>(Items[0])->GetGameObject()->GetWorld();
		for (auto& Item : Items)
		{
			NxFr::Handle<NxEn::GameObject> Instance = static_cast<HierarchyItem*>(Item)->GetGameObject();

			World->DuplicateGameObject(Instance);
		}
	}
}
