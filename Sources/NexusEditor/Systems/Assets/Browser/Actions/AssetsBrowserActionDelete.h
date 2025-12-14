#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionDelete : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionDelete(NxFr::StringView Label, int64 Priority = 0);
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionDelete();

		NEXUS_EDITOR_API void Execute(AssetsBrowserItem* Item) override;
	};
}
