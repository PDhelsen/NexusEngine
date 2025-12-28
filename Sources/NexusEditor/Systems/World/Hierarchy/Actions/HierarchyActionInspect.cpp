#include "NexusEditor/Systems/World/Hierarchy/Actions/HierarchyActionInspect.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Inspector/InspectorPanel.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyActionInspect)

	HierarchyActionInspect::HierarchyActionInspect()
		: HierarchyAction("Inspect", 9)
	{
	}

	HierarchyActionInspect::~HierarchyActionInspect()
	{
	}

	void HierarchyActionInspect::Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items)
	{
		NxEn::GUISystem::GetPanel<InspectorPanel>()->Show(Items[0]);
	}
}
