#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionReimport : public AssetsBrowserAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReimport)

		NEXUS_EDITOR_API AssetsBrowserActionReimport();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionReimport();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;
	};
}

