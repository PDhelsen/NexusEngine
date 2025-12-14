#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionCreate : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionCreate(NxFr::StringView Label, int64 Priority = 0);
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionCreate();

		NEXUS_EDITOR_API void Execute(AssetsBrowserItem* Item) override;
	};
}
