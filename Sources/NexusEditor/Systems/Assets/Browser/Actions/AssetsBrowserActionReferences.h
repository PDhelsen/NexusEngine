#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionReferences : public AssetsBrowserAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReferences)

		NEXUS_EDITOR_API AssetsBrowserActionReferences();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionReferences();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;
	};
}

