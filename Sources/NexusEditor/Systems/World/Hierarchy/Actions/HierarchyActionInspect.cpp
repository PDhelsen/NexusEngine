#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionInspect.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Inspector/InspectorPanel.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionInspect)

	HierarchyActionInspect::HierarchyActionInspect()
		: TreeAction("Inspect", 9, false, false)
	{
	}

	HierarchyActionInspect::~HierarchyActionInspect()
	{
	}

	void HierarchyActionInspect::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxFr::Handle<NxEn::GameObject> Item = static_cast<HierarchyItem*>(Items[0])->GetGameObject();
		NxEn::GUISystem::GetPanel<InspectorPanel>()->Show(Item);
	}
}
