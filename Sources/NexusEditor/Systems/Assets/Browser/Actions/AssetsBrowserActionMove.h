#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionMove : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionMove(NxFr::StringView Label, int64 Priority = 0);
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionMove();

		NEXUS_EDITOR_API void Execute(AssetsBrowserItem* Item) override;
	};
}
