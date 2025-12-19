#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyAction)

	HierarchyAction::HierarchyAction(NxFr::StringView Label, int64 Priority, bool Recursive, bool LastSelectedFirst)
		: Label(Label), Priority(Priority), Recursive(Recursive), LastSelectedFirst(LastSelectedFirst)
	{
	}

	HierarchyAction::~HierarchyAction()
	{
	}

	bool HierarchyAction::operator<=(const HierarchyAction& Other)
	{
		return Priority <= Other.Priority;
	}
}
