#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	AssetsBrowserAction::AssetsBrowserAction(NxFr::StringView Label, int64 Priority, bool Recursive)
		: Label(Label), Priority(Priority), Recursive(Recursive)
	{
	}

	AssetsBrowserAction::~AssetsBrowserAction()
	{
	}

	bool AssetsBrowserAction::operator<=(const AssetsBrowserAction& Other)
	{
		return Priority <= Other.Priority;
	}
}
