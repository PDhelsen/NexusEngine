#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/Tree/TreeAction.h"

namespace NxEn
{
	TreeAction::TreeAction(NxFr::StringView Label, int64 Priority, bool Recursive, bool LastSelectedFirst)
		: Label(Label), Priority(Priority), Recursive(Recursive), LastSelectedFirst(LastSelectedFirst)
	{
	}

	TreeAction::~TreeAction()
	{
	}
}
