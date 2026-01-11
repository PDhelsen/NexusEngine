#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(TreeAction)

	TreeAction::TreeAction(NxFr::StringView Label, int64 Priority, bool Recursive, bool LastSelectedFirst)
		: Label(Label), Priority(Priority), Recursive(Recursive), LastSelectedFirst(LastSelectedFirst)
	{
	}

	TreeAction::~TreeAction()
	{
	}
}
