#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionDuplicate : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionDuplicate(NxFr::StringView Label, int64 Priority = 0);
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionDuplicate();

		NEXUS_EDITOR_API void Execute(AssetsBrowserItem* Item) override;
	};
}
