#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionReload : public AssetsBrowserAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReload)

		NEXUS_EDITOR_API AssetsBrowserActionReload();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionReload();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;
	};
}

