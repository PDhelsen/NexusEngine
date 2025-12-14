#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	AssetsBrowserAction::AssetsBrowserAction(NxFr::StringView Label, int64 Priority)
		: Label(Label), Priority(Priority)
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
